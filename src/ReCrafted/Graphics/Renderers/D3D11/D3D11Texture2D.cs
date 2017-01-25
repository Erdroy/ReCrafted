// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using SharpDX;
using SharpDX.Direct3D;
using SharpDX.Direct3D11;
using Rectangle = System.Drawing.Rectangle;

namespace ReCrafted.Graphics.Renderers.D3D11
{
    /// <summary>
    /// D3D11Texture2D class.
    /// </summary>
    internal sealed class D3D11Texture2D : Texture2D
    {
        /// <summary>
        /// Texture2D
        /// </summary>
        public SharpDX.Direct3D11.Texture2D Texture2D;

        /// <summary>
        /// ResourceView
        /// </summary>
        public ShaderResourceView ResourceView;

        /// <summary>
        /// Load texture from bitmap.
        /// </summary>
        /// <param name="bitmap">The bitmap.</param>
        /// <param name="genMips">Generate mipmaps for this texture?</param>
        /// <param name="maxMips">The maximal count of mipmaps.</param>
        /// <param name="samplerType">The sampler type of the texture.</param>
        protected override void Load(Bitmap bitmap, bool genMips, int maxMips, Sampler.Type samplerType)
        {
            var stride = bitmap.Width * 4;
            using (var buffer = new DataStream(bitmap.Height*stride, true, true))
            {
                var lockedBits = bitmap.LockBits(new Rectangle(0, 0, bitmap.Width, bitmap.Height), 
                    ImageLockMode.ReadOnly,
                    PixelFormat.Format32bppArgb);
                
                buffer.Write(lockedBits.Scan0, 0, bitmap.Height * stride);
                
                Texture2D = new SharpDX.Direct3D11.Texture2D(D3D11Renderer.GetDevice(), new Texture2DDescription
                    {
                        Width = bitmap.Width,
                        Height = bitmap.Height,
                        ArraySize = 1,
                        BindFlags = BindFlags.ShaderResource | BindFlags.RenderTarget,
                        Usage = ResourceUsage.Default,
                        CpuAccessFlags = CpuAccessFlags.None,
                        Format = SharpDX.DXGI.Format.B8G8R8A8_UNorm,
                        MipLevels = maxMips,
                        OptionFlags = ResourceOptionFlags.GenerateMipMaps,
                        SampleDescription = new SharpDX.DXGI.SampleDescription(1, 0),
                    });
                
                var box = new DataBox(buffer.DataPointer, stride, 1);
                D3D11Renderer.GetDeviceContext().UpdateSubresource(box, Texture2D);

                ResourceView = new ShaderResourceView(D3D11Renderer.GetDevice(), Texture2D, new ShaderResourceViewDescription
                {
                    Dimension = ShaderResourceViewDimension.Texture2D,
                    Texture2D = new ShaderResourceViewDescription.Texture2DResource
                    {
                        MipLevels = maxMips,
                        MostDetailedMip = 0
                    }
                });

                if(genMips)
                    D3D11Renderer.GetDeviceContext().GenerateMips(ResourceView);
            }
        }
        
        /// <summary>
        /// Dispose the texture.
        /// </summary>
        public override void Dispose()
        {
            Texture2D?.Dispose();
            ResourceView?.Dispose();
        }
    }
}