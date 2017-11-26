// ReCrafted © 2016-2017 Always Too Late

using System;
using System.Windows.Forms;
using SharpDX;
using SharpDX.Direct3D;
using SharpDX.Direct3D11;
using SharpDX.DXGI;
using SharpDX.Windows;
using Device = SharpDX.Direct3D11.Device;
using Resource = SharpDX.Direct3D11.Resource;

namespace ReCrafted.VoxelEditor.Rendering
{
    public class Renderer : IDisposable
    {
        internal Device D3D11Device;
        internal DeviceContext DeviceContext;
        internal SwapChain SwapChain;

        private RenderForm _window;
        private Shader _defaultShader;
        private Texture2D _backBuffer;
        private RenderTargetView _renderView;

        public void Init()
        {
            Current = this;

            // create render window
            _window = new RenderForm("ReCrafted - VoxelEditor")
            {
                AllowUserResizing = true,
                WindowState = FormWindowState.Maximized
            };

            _window.UserResized += (sender, args) => { Resize(); };
            _window.Show();

            // create device
            Device.CreateWithSwapChain(DriverType.Hardware, DeviceCreationFlags.Debug, new [] { FeatureLevel.Level_11_0 }, new SwapChainDescription
            {
                BufferCount = 1,
                ModeDescription =
                    new ModeDescription(
                        _window.ClientSize.Width, _window.ClientSize.Height, new Rational(60, 1), Format.R8G8B8A8_UNorm
                        ),
                IsWindowed = true,
                OutputHandle = _window.Handle,
                SampleDescription = new SampleDescription(1, 0),
                SwapEffect = SwapEffect.Discard,
                Usage = Usage.RenderTargetOutput
            }, out D3D11Device, out SwapChain);

            DeviceContext = D3D11Device.ImmediateContext;

            var factory = SwapChain.GetParent<Factory>();
            factory.MakeWindowAssociation(_window.Handle, WindowAssociationFlags.IgnoreAll);

            // create default render targets etc.
            Resize();

            // load default shader
            _defaultShader = Shader.FromFile("assets/shaders/default.hlsl");
        }

        public void BeginFrame()
        {
            // do application events
            Application.DoEvents();

            // set everything
            DeviceContext.InputAssembler.PrimitiveTopology = PrimitiveTopology.TriangleList;

            // apply shader
            _defaultShader.Apply();

            // TODO: blend state

            DeviceContext.OutputMerger.SetTargets(_renderView);
            DeviceContext.ClearRenderTargetView(_renderView, Color.Black);
        }

        public void EndFrame()
        {
            SwapChain.Present(1, PresentFlags.None);
        }

        public void Resize()
        {
            // check if we ar already initialized, if not, exit.
            if (D3D11Device == null)
                return;

            var width = _window.ClientSize.Width;
            var height = _window.ClientSize.Height;
            
            Utilities.Dispose(ref _backBuffer);
            Utilities.Dispose(ref _renderView);

            SwapChain.ResizeBuffers(1, width, height, Format.Unknown, SwapChainFlags.None);

            // create back buffer
            _backBuffer = Resource.FromSwapChain<Texture2D>(SwapChain, 0);

            // create view
            _renderView = new RenderTargetView(D3D11Device, _backBuffer);

            // resize view port
            DeviceContext.Rasterizer.SetViewport(new Viewport(0, 0, width, height, 0.0f, 1.0f));
        }

        public void Dispose()
        {
            _window.Close();

            // dispose all resources
            _backBuffer.Dispose();
            _renderView.Dispose();
            _defaultShader.Dispose();
            _window.Dispose();
            SwapChain.Dispose();
            D3D11Device.Dispose();
        }
        

        /// <summary>
        /// The render form.
        /// </summary>
        public Form RenderForm => _window;

        internal static Renderer Current { get; private set; }
    }
}
