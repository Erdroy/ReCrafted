// ReCrafted © 2016 Damian 'Erdroy' Korczowski

using SharpDX;
using SharpDX.Direct3D11;
using SharpDX.DXGI;

namespace ReCrafted.Graphics.Renderers.D3D11
{
    /// <summary>
    /// D3D11RenderTarget class.
    /// </summary>
    public sealed class D3D11RenderTarget : RenderTarget
    {
        internal SharpDX.Direct3D11.Texture2D Texture2D;
        internal RenderTargetView View;
        internal ShaderResourceView ResourceView;
        internal UnorderedAccessView UnorderedAccessView;

        /// <summary>
        /// D3D11RenderTarget constructor.
        /// </summary>
        public D3D11RenderTarget(int width, int height, bool uav)
        {
            Internal_Create(width, height, uav);
        }

        /// <summary>
        /// Resize the render target.
        /// </summary>
        public override void Resize(int width, int height)
        {
            var uav = UnorderedAccessView != null;
            Dispose();
            Internal_Create(width, height, uav);
        }

        /// <summary>
        /// Clear the render target using given color.
        /// </summary>
        /// <param name="color">The color.</param>
        public override void Clear(Color color)
        {
            D3D11Renderer.GetDevice().ImmediateContext.ClearRenderTargetView(View, color);
        }

        /// <summary>
        /// Dispose the RenderTarget.
        /// </summary>
        public override void Dispose()
        {
            Texture2D?.Dispose();
            View?.Dispose();

            ResourceView?.Dispose();
            UnorderedAccessView?.Dispose();
        }

        // private
        private void Internal_Create(int width, int height, bool uav)
        {
            if (uav)
            {
                Texture2D = new SharpDX.Direct3D11.Texture2D(D3D11Renderer.GetDevice(), new Texture2DDescription
                {
                    BindFlags = BindFlags.RenderTarget | BindFlags.ShaderResource | BindFlags.UnorderedAccess,
                    Width = width,
                    Height = height,
                    MipLevels = 1,
                    ArraySize = 1,
                    CpuAccessFlags = CpuAccessFlags.None,
                    Format = Format.R8G8B8A8_UNorm,
                    OptionFlags = ResourceOptionFlags.None,
                    SampleDescription = new SampleDescription(1, 0),
                    Usage = ResourceUsage.Default
                });

                UnorderedAccessView = new UnorderedAccessView(D3D11Renderer.GetDevice(), Texture2D);
            }
            else
            {
                Texture2D = new SharpDX.Direct3D11.Texture2D(D3D11Renderer.GetDevice(), new Texture2DDescription
                {
                    BindFlags = BindFlags.RenderTarget | BindFlags.ShaderResource,
                    Width = width,
                    Height = height,
                    MipLevels = 1,
                    ArraySize = 1,
                    CpuAccessFlags = CpuAccessFlags.None,
                    Format = Format.R8G8B8A8_UNorm,
                    OptionFlags = ResourceOptionFlags.None,
                    SampleDescription = new SampleDescription(1, 0),
                    Usage = ResourceUsage.Default
                });
            }
            
            View = new RenderTargetView(D3D11Renderer.GetDevice(), Texture2D);
            ResourceView = new ShaderResourceView(D3D11Renderer.GetDevice(), Texture2D);
        }
    }
}
