// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

using System;
using ReCrafted.Graphics.Renderers.D3D11;
using ReCrafted.Utilities;
using SharpDX;

namespace ReCrafted.Graphics
{
    /// <summary>
    /// RenderTarget class.
    /// </summary>
    public abstract class RenderTarget : IDisposable
    {
        // ReSharper disable InconsistentNaming
        /// <summary>
        /// The RenderTarget format.
        /// </summary>
        public enum TextureFormat
        {
            R16_Float,
            R32_Float,
            RG32_Float,
            RGB32_Float,
            RGBA32_Float,

            RGBA8_UNorm,

            Depth
        }
        // ReSharper restore InconsistentNaming

        /// <summary>
        /// RenderTarget constructor.
        /// </summary>
        protected RenderTarget() { }

        /// <summary>
        /// Resize the render target.
        /// </summary>
        public abstract void Resize(int width, int height);

        /// <summary>
        /// Clear the render target using given color.
        /// </summary>
        /// <param name="color">The color.</param>
        public abstract void Clear(Color color);

        /// <summary>
        /// Dispose the RenderTarget.
        /// </summary>
        public abstract void Dispose();

        /// <summary>
        /// Create new RenderTarget.
        /// </summary>
        /// <param name="width">The width of the RenderTarget.</param>
        /// <param name="height">The height of the RenderTarget.</param>
        /// <param name="format">The format.</param>
        /// <param name="uav">Create Unordered access view?</param>
        /// <returns>The created RenderTarget.</returns>
        public static RenderTarget Create(int width, int height, TextureFormat format, bool uav = false)
        {
            switch (Renderer.RendererApi)
            {
                case RendererApi.D3D11:
                    return new D3D11RenderTarget(width, height, format, uav);
            }

            // TODO: Implement renderers

            throw new ReCraftedException("Invalid RendererApi!");
        }
    }
}