// ReCrafted © 2016-2017 Always Too Late

using System;
using System.Windows.Forms;
using SharpDX.Direct3D;
using SharpDX.Direct3D11;
using SharpDX.DXGI;
using SharpDX.Windows;
using Device = SharpDX.Direct3D11.Device;

namespace ReCrafted.VoxelEditor.Rendering
{
    public class Renderer : IDisposable
    {
        private Device _device;
        private DeviceContext _deviceContext;
        private SwapChain _swapChain;
        private RenderForm _window;

        public void Init()
        {
            // create render window
            _window = new RenderForm("ReCrafted - VoxelEditor")
            {
                AllowUserResizing = true,
                WindowState = FormWindowState.Maximized
            };
            
            _window.Show();

            // create device
            Device.CreateWithSwapChain(DriverType.Hardware, DeviceCreationFlags.None, new [] { FeatureLevel.Level_11_0 }, new SwapChainDescription
            {
                BufferCount = 1,
                ModeDescription =
                    new ModeDescription(_window.ClientSize.Width, _window.ClientSize.Height,
                        new Rational(60, 1), Format.R8G8B8A8_UNorm),
                IsWindowed = true,
                OutputHandle = _window.Handle,
                SampleDescription = new SampleDescription(1, 0),
                SwapEffect = SwapEffect.Discard,
                Usage = Usage.RenderTargetOutput
            }, out _device, out _swapChain);

            _deviceContext = _device.ImmediateContext;
        }

        public void BeginFrame()
        {
            // do application events
            Application.DoEvents();
            
        }

        public void EndFrame()
        {
            _swapChain.Present(1, PresentFlags.None);
        }

        public void Dispose()
        {
            _window.Close();

            // dispose all resources
            _window.Dispose();
            _swapChain.Dispose();
            _device.Dispose();
        }

        /// <summary>
        /// The render form.
        /// </summary>
        public Form RenderForm => _window;
    }
}
