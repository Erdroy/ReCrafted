// ReCrafted © 2016 Damian 'Erdroy' Korczowski

using System;
using ReCrafted.Utilities;
using SharpDX;
using SharpDX.Direct3D;
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
        internal RenderTargetView TextureView;
        internal DepthStencilView DepthView;
        internal ShaderResourceView ResourceView;
        internal UnorderedAccessView UnorderedAccessView;

        public TextureFormat TexFormat;

        /// <summary>
        /// D3D11RenderTarget constructor.
        /// </summary>
        public D3D11RenderTarget(int width, int height, TextureFormat format, bool uav)
        {
            TexFormat = format;

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
            D3D11Renderer.GetDevice().ImmediateContext.ClearRenderTargetView(TextureView, color);
        }

        /// <summary>
        /// Dispose the RenderTarget.
        /// </summary>
        public override void Dispose()
        {
            Texture2D?.Dispose();
            TextureView?.Dispose();

            ResourceView?.Dispose();
            UnorderedAccessView?.Dispose();
        }

        // private
        private void Internal_Create(int width, int height, bool uav)
        {
            if (TexFormat == TextureFormat.Depth)
            {
                if (uav)
                {
                    throw new ReCraftedException("Cannot make depth rt as UAV!");
                }

                var device = D3D11Renderer.GetDevice();

                // create depth buffer
                Texture2D = new SharpDX.Direct3D11.Texture2D(device, new Texture2DDescription()
                {
                    Width = width,
                    Height = height,
                    Format = Format.R24G8_Typeless,
                    ArraySize = 1,
                    MipLevels = 1,
                    SampleDescription = new SampleDescription(1, 0),
                    Usage = ResourceUsage.Default,
                    BindFlags = BindFlags.ShaderResource | BindFlags.DepthStencil,
                    CpuAccessFlags = CpuAccessFlags.None,
                    OptionFlags = ResourceOptionFlags.None
                });

                // create the depth buffer view
                DepthView = new DepthStencilView(device, Texture2D, new DepthStencilViewDescription
                {
                    Flags = DepthStencilViewFlags.None,
                    Format = Format.D24_UNorm_S8_UInt,
                    Dimension = DepthStencilViewDimension.Texture2D,
                    Texture2D = new DepthStencilViewDescription.Texture2DResource()
                    {
                        MipSlice = 0
                    }
                });
                
                // create res view
                ResourceView = new ShaderResourceView(device, Texture2D, new ShaderResourceViewDescription()
                {
                    Format = Format.R24_UNorm_X8_Typeless,
                    Dimension = ShaderResourceViewDimension.Texture2D,

                    Texture2D = new ShaderResourceViewDescription.Texture2DResource
                    {
                        MipLevels = 1,
                        MostDetailedMip = 0
                    }
                });
                return;
            }

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
                    Format = GetFormat(),
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
                    Format = GetFormat(),
                    OptionFlags = ResourceOptionFlags.None,
                    SampleDescription = new SampleDescription(1, 0),
                    Usage = ResourceUsage.Default
                });
            }
            
            TextureView = new RenderTargetView(D3D11Renderer.GetDevice(), Texture2D);
            ResourceView = new ShaderResourceView(D3D11Renderer.GetDevice(), Texture2D);
        }

        private Format GetFormat()
        {
            switch (TexFormat)
            {
                case TextureFormat.R32_Float:
                    return Format.R32_Float;
                case TextureFormat.RG32_Float:
                    return Format.R32G32_Float;
                case TextureFormat.RGB32_Float:
                    return Format.R32G32B32_Float;
                case TextureFormat.RGBA32_Float:
                    return Format.R32G32B32A32_Float;

                case TextureFormat.RGBA8_UNorm:
                    return Format.R8G8B8A8_UNorm;

                case TextureFormat.Depth:
                    return Format.D32_Float_S8X24_UInt;

                default:
                    throw new ArgumentOutOfRangeException();
            }
        }
    }
}
