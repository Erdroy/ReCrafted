// ReCrafted © 2016 Damian 'Erdroy' Korczowski

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

        private SharpDX.Direct3D11.Texture2D _depthBuffer;

        private RenderTargetView _finalRenderTarget;
        private DepthStencilView _depthStencilView;

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
            
            _device.ImmediateContext.OutputMerger.SetTargets(_depthStencilView, _currentViews);
            _device.ImmediateContext.ClearDepthStencilView(_depthStencilView, DepthStencilClearFlags.Depth, 1.0f, 0);
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
            _depthBuffer?.Dispose();
            _depthStencilView?.Dispose();

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
            _depthBuffer = new SharpDX.Direct3D11.Texture2D(_device, new Texture2DDescription()
            {
                Format = Format.D32_Float_S8X24_UInt,
                ArraySize = 1,
                MipLevels = 1,
                Width = width,
                Height = height,
                SampleDescription = new SampleDescription(1, 0),
                Usage = ResourceUsage.Default,
                BindFlags = BindFlags.DepthStencil,
                CpuAccessFlags = CpuAccessFlags.None,
                OptionFlags = ResourceOptionFlags.None
            });

            // create the depth buffer view
            _depthStencilView = new DepthStencilView(_device, _depthBuffer);
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
        /// Sets the depth test state.
        /// </summary>
        /// <param name="enabled">Disable or enable the depth test?</param>
        public override void SetDepthTestState(bool enabled)
        {
            if (enabled)
            {
                _device.ImmediateContext.OutputMerger.SetTargets(_depthStencilView, _currentViews);
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
            _currentViews = renderTargets.Select(renderTarget => ((D3D11RenderTarget) renderTarget).View).ToArray();
            _device.ImmediateContext.OutputMerger.SetTargets(_depthStencilView, _currentViews);
        }

        /// <summary>
        /// Set the final render target.
        /// </summary>
        /// <param name="useDepthTest">Use DepthTest?</param>
        public override void SetFinalRenderTarget(bool useDepthTest)
        {
            if (useDepthTest)
            {
                _device.ImmediateContext.OutputMerger.SetTargets(_depthStencilView, _finalRenderTarget);
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
        public override void Blit(RenderTarget renderTarget)
        {
            var deviceContext = _device.ImmediateContext;
            var shd = (D3D11Shader) _blitShader;

            // apply shader and all values/resources
            shd.Apply();
            shd.SetRenderTexture(0, renderTarget);

            // draw
            deviceContext.InputAssembler.PrimitiveTopology = PrimitiveTopology.TriangleStrip;
            deviceContext.Draw(4, 0);

            shd.UnsetRenderTexture(0);
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
            _depthBuffer?.Dispose();
            _depthStencilView?.Dispose();
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