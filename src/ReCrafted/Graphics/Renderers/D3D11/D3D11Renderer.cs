// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

using System;
using System.Linq;
using System.Windows.Forms;
using ReCrafted.Core;
using ReCrafted.Utilities;
using SharpDX;
using SharpDX.Direct3D;
using SharpDX.Direct3D11;
using SharpDX.DXGI;
using Device = SharpDX.Direct3D11.Device;

namespace ReCrafted.Graphics.Renderers.D3D11
{
    /// <summary>
    /// D3D11Renderer class.
    /// </summary>
    internal sealed class D3D11Renderer : Renderer
    {
        /// <summary>
        /// The D3D11Renderer instance.
        /// </summary>
        public new static D3D11Renderer Instance;

        private Device _device;
        private SwapChain _swapChain;
        
        private RenderTargetView _finalRenderTarget;

        private RasterizerState _wireframeRasterizerState;
        private RasterizerState _defaultRasterizerState;
        
        private RenderTargetView[] _currentViews;

        private Shader _colorShader;
        private Shader _blitShader;

        private Mesh _boundingBox;

        /// <summary>
        /// Initializes the renderer.
        /// </summary>
        protected override void Init()
        {
            Instance = this;
            try
            {
                Device.CreateWithSwapChain(DriverType.Hardware, DeviceCreationFlags.Debug,
                    new[] {FeatureLevel.Level_11_1}, new SwapChainDescription
                    {
                        BufferCount = 1,
                        Flags = SwapChainFlags.None,
                        IsWindowed = true,
                        ModeDescription =
                            new ModeDescription(Display.ClientWidth, Display.ClientHeight, new Rational(60, 1),
                                Format.R8G8B8A8_UNorm),
                        OutputHandle = Game.Instance.Form.Handle,
                        SampleDescription = new SampleDescription(1, 0),
                        SwapEffect = SwapEffect.Discard,
                        Usage = Usage.RenderTargetOutput
                    }, out _device, out _swapChain);
            }
            catch (Exception ex)
            {
                MessageBox.Show("" + ex);
                Environment.Exit(0);
            }

            Resize(Display.ClientWidth, Display.ClientHeight);

            var rsd = new RasterizerStateDescription()
            {
                CullMode = CullMode.Back,
                DepthBias = 0,
                DepthBiasClamp = 0.0f,
                FillMode = FillMode.Wireframe,
                IsAntialiasedLineEnabled = false,
                IsDepthClipEnabled = false,
                IsMultisampleEnabled = false,
                IsScissorEnabled = false,
                SlopeScaledDepthBias = 0.0f
            };

            _wireframeRasterizerState = new RasterizerState(_device, rsd);
            _defaultRasterizerState = _device.ImmediateContext.Rasterizer.State;

            _colorShader = Shader.FromFile("internal/Color");
            _blitShader = Shader.FromFile("internal/Blit");

            // create bounding box mesh

            #region Bouding box mesh
            var bounds = new BoundingBox(Vector3.Zero, Vector3.One);
            _boundingBox = Mesh.Create();
            _boundingBox.PrimitiveType = PrimitiveType.LineList;
            _boundingBox.SetVertices(new[]
            {
                bounds.Minimum,
                bounds.Minimum + Vector3.ForwardLH,

                bounds.Minimum,
                bounds.Minimum + Vector3.Right,

                bounds.Minimum,
                bounds.Minimum + Vector3.Up,

                bounds.Minimum + Vector3.Up,
                bounds.Minimum + Vector3.Up + Vector3.ForwardLH,

                bounds.Minimum + Vector3.Up,
                bounds.Minimum + Vector3.Up + Vector3.Right,

                bounds.Maximum,
                bounds.Maximum - Vector3.Up,

                bounds.Maximum,
                bounds.Maximum - Vector3.ForwardLH,

                bounds.Maximum,
                bounds.Maximum - Vector3.Right,

                bounds.Maximum - Vector3.ForwardLH,
                bounds.Maximum - Vector3.Up - Vector3.ForwardLH,

                bounds.Maximum - Vector3.Right,
                bounds.Maximum - Vector3.Up - Vector3.Right,

                bounds.Maximum - Vector3.Up,
                bounds.Maximum - Vector3.Up - Vector3.ForwardLH,

                bounds.Maximum - Vector3.Up,
                bounds.Maximum - Vector3.Up - Vector3.Right,

            });
            _boundingBox.SetColors(new[]
            {
                Color.OrangeRed,
                Color.OrangeRed,

                Color.OrangeRed,
                Color.OrangeRed,

                Color.OrangeRed,
                Color.OrangeRed,

                Color.OrangeRed,
                Color.OrangeRed,

                Color.OrangeRed,
                Color.OrangeRed,

                Color.OrangeRed,
                Color.OrangeRed,

                Color.OrangeRed,
                Color.OrangeRed,

                Color.OrangeRed,
                Color.OrangeRed,

                Color.OrangeRed,
                Color.OrangeRed,

                Color.OrangeRed,
                Color.OrangeRed,

                Color.OrangeRed,
                Color.OrangeRed,

                Color.OrangeRed,
                Color.OrangeRed,
            });
            _boundingBox.ApplyChanges();
            #endregion
        }

        /// <summary>
        /// Ticks the renderer.
        /// </summary>
        public override void Tick()
        {

        }

        /// <summary>
        /// Draws new frame.
        /// </summary>
        public override void Draw()
        {
            _currentViews = new []
            {
                _finalRenderTarget
            };

            var depthrt = (D3D11RenderTarget) DepthRenderTarget;

            _device.ImmediateContext.OutputMerger.SetTargets(depthrt.DepthView, _currentViews);
            _device.ImmediateContext.ClearDepthStencilView(depthrt.DepthView, DepthStencilClearFlags.Depth, 1.0f, 0);
            _device.ImmediateContext.ClearRenderTargetView(_finalRenderTarget, Camera.Current.BackgroundColor);
            
            Rendering.Draw();

            _swapChain.Present(1, 0);
        }
        
        /// <summary>
        /// Reizes the targets.
        /// </summary>
        /// <param name="width">Width</param>
        /// <param name="height">Height</param>
        public override void Resize(int width, int height)
        {
            _finalRenderTarget?.Dispose();
            DepthRenderTarget?.Dispose();

            if (width < 32)
                width = 32;

            if (height < 32)
                height = 32;

            // set viewport
            _device.ImmediateContext.Rasterizer.SetViewport(0.0f, 0.0f, width, height);

            // create new buffers

            // resize buffers
            _swapChain.ResizeBuffers(1, width, height, Format.Unknown, 0);

            using (var backBuffer = _swapChain.GetBackBuffer<SharpDX.Direct3D11.Texture2D>(0))
            {
                _finalRenderTarget = new RenderTargetView(_device, backBuffer);
            }

            // create depth buffer
            DepthRenderTarget = new D3D11RenderTarget(width, height, RenderTarget.TextureFormat.Depth, false);
        }

        /// <summary>
        /// Sets rasterizer state.
        /// </summary>
        /// <param name="state">The state.</param>
        public override void SetRasterizerState(RasterizerStates state)
        {
            switch (state)
            {
                case RasterizerStates.BackFaceCulled:
                    _device.ImmediateContext.Rasterizer.State = _defaultRasterizerState;
                    break;
                case RasterizerStates.Wireframe:
                    _device.ImmediateContext.Rasterizer.State = _wireframeRasterizerState;
                    break;

                default:
                    throw new ReCraftedException("Not implemented rasterizer state in current renderer.");
            }
        }

        /// <summary>
        /// Set the viewport size.
        /// </summary>
        /// <param name="width">The width.</param>
        /// <param name="height">The height.</param>
        public override void SetViewportSize(int width, int height)
        {
            _device.ImmediateContext.Rasterizer.SetViewport(0.0f, 0.0f, width, height);
        }

        /// <summary>
        /// Sets the depth test state.
        /// </summary>
        /// <param name="enabled">Disable or enable the depth test?</param>
        public override void SetDepthTestState(bool enabled)
        {
            if (enabled)
            {
                var depthrt = (D3D11RenderTarget)DepthRenderTarget;
                _device.ImmediateContext.OutputMerger.SetTargets(depthrt.DepthView, _currentViews);
            }
            else
            {
                _device.ImmediateContext.OutputMerger.SetTargets(_currentViews);
            }
        }

        /// <summary>
        /// Set RenderTargets as the current frame output.
        /// </summary>
        /// <param name="renderTargets">The RenderTargets.</param>
        public override void SetRenderTargets(params RenderTarget[] renderTargets)
        {
            var depthrt = (D3D11RenderTarget)DepthRenderTarget;
            _currentViews = renderTargets.Select(renderTarget => ((D3D11RenderTarget) renderTarget).TextureView).ToArray();
            _device.ImmediateContext.OutputMerger.SetTargets(depthrt.DepthView, _currentViews);
        }

        /// <summary>
        /// Set RenderTargets as the current frame output.
        /// </summary>
        /// <param name="depthRenderTarget">The depth render target.</param>
        /// <param name="renderTargets">The RenderTargets.</param>
        public override void SetRenderTargetsDepth(RenderTarget depthRenderTarget, params RenderTarget[] renderTargets)
        {
            var depthrt = (D3D11RenderTarget)depthRenderTarget;
            _currentViews = renderTargets.Select(renderTarget => ((D3D11RenderTarget)renderTarget).TextureView).ToArray();
            _device.ImmediateContext.OutputMerger.SetTargets(depthrt.DepthView, _currentViews);
        }

        /// <summary>
        /// Set the final render target.
        /// </summary>
        /// <param name="useDepthTest">Use DepthTest?</param>
        public override void SetFinalRenderTarget(bool useDepthTest)
        {
            if (useDepthTest)
            {
                var depthrt = (D3D11RenderTarget)DepthRenderTarget;
                _device.ImmediateContext.OutputMerger.SetTargets(depthrt.DepthView, _finalRenderTarget);
            }
            else
            {
                _device.ImmediateContext.OutputMerger.SetTargets(_finalRenderTarget);
            }
        }

        /// <summary>
        /// Render/Copy the given RenderTarget to the current set RenderTarget.
        /// </summary>
        /// <param name="renderTarget">The render target.</param>
        /// <param name="customShader">The custom shader for blit.</param>
        public override void Blit(RenderTarget renderTarget, Shader customShader = null)
        {
            var deviceContext = _device.ImmediateContext;
            var shd = (D3D11Shader) _blitShader;

            if (customShader != null)
                shd = (D3D11Shader)customShader;

            // apply shader and all values/resources
            if (customShader == null)
                shd.Apply();

            if(renderTarget != null)
                shd.SetRenderTexture(ShaderType.PS, 0, renderTarget);

            // draw
            deviceContext.InputAssembler.PrimitiveTopology = PrimitiveTopology.TriangleStrip;
            deviceContext.Draw(4, 0);

            if (renderTarget != null)
                shd.UnsetRenderTexture(ShaderType.PS, 0);
        }

        /// <summary>
        /// Clears depth.
        /// </summary>
        public override void ClearDepth()
        {
            var depthrt = (D3D11RenderTarget)DepthRenderTarget;
            _device.ImmediateContext.ClearDepthStencilView(depthrt.DepthView, DepthStencilClearFlags.Depth, 1.0f, 0);
        }

        /// <summary>
        /// Changes face culling
        /// </summary>
        public override void FaceCulling(bool front, bool back)
        {
            // TODO: d3d11 face culling change
        }

        /// <summary>
        /// Dispatch ComputeShader.
        /// </summary>
        /// <param name="x">The X amount of threads.</param>
        /// <param name="y">The Y amount of threads.</param>
        /// <param name="z">The Z amount of threads.</param>
        public override void Dispatch(int x, int y, int z)
        {
            var deviceContext = _device.ImmediateContext;
            deviceContext.Dispatch(x, y, z);
        }

        /// <summary>
        /// Gets all RenderTargets from the current frame output.
        /// </summary>
        /// <returns>The RenderTargets.</returns>
        public override RenderTarget[] GetRenderTargets()
        {
            throw new NotImplementedException("GetRenderTargets() is not implemented currently!");
        }

        /// <summary>
        /// Draw bounding box.
        /// </summary>
        /// <param name="bounds">The bouding box.</param>
        public override void DrawBoundingBox(BoundingBox bounds)
        {
            var scale = bounds.Maximum - bounds.Minimum;

            var wvp = Matrix.Scaling(scale) * Camera.Current.ViewProjectionMatrix;
            wvp.Transpose();
            _colorShader.SetValue("WVP", wvp);
            _colorShader.Draw(_boundingBox);

        }

        /// <summary>
        /// Dispose the renderer.
        /// </summary>
        public override void Dispose()
        {
            DepthRenderTarget?.Dispose();
            _device?.Dispose();
            _finalRenderTarget?.Dispose();
            _swapChain?.Dispose();
            _boundingBox?.Dispose();

            _blitShader?.Dispose();
            _colorShader?.Dispose();
            _boundingBox?.Dispose();
        }

        /// <summary>
        /// Gets device.
        /// </summary>
        /// <returns>The Device</returns>
        public static Device GetDevice()
        {
            return Instance._device;
        }

        /// <summary>
        /// Gets device context.
        /// </summary>
        /// <returns>The DeviceContext.</returns>
        public static DeviceContext GetDeviceContext()
        {
            return Instance._device.ImmediateContext;
        }
    }
}