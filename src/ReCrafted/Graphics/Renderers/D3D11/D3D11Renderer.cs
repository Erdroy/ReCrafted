using ReCrafted.Core;
using ReCrafted.Logic;
using ReCrafted.Utilities;
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

        /// <summary>
        /// Initializes the renderer.
        /// </summary>
        protected override void Init()
        {
            Instance = this;

            Device.CreateWithSwapChain(DriverType.Hardware, DeviceCreationFlags.Debug, new []{FeatureLevel.Level_11_1}, new SwapChainDescription
            {
                BufferCount = 1,
                Flags = SwapChainFlags.None,
                IsWindowed = true,
                ModeDescription = new ModeDescription(Display.ClientWidth, Display.ClientHeight, new Rational(60, 1), Format.R8G8B8A8_UNorm),
                OutputHandle = Game.Instance.Form.Handle,
                SampleDescription = new SampleDescription(1, 0),
                SwapEffect = SwapEffect.Discard,
                Usage = Usage.RenderTargetOutput
            }, out _device, out _swapChain);

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
            
            base.Init();
        }
        
        /// <summary>
        /// Draws new frame.
        /// </summary>
        public override void Draw()
        {
            _device.ImmediateContext.OutputMerger.SetTargets(_depthStencilView, _finalRenderTarget);
            _device.ImmediateContext.ClearDepthStencilView(_depthStencilView, DepthStencilClearFlags.Depth, 1.0f, 0);
            _device.ImmediateContext.ClearRenderTargetView(_finalRenderTarget, Camera.Current.BackgroundColor);
            
            Scene.Instance.Draw();

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
        /// Dispose the renderer.
        /// </summary>
        public override void Dispose()
        {
            _depthBuffer?.Dispose();
            _depthStencilView?.Dispose();
            _device?.Dispose();
            _finalRenderTarget?.Dispose();
            _swapChain?.Dispose();
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