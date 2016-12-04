// ReCrafted © 2016 Damian 'Erdroy' Korczowski

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
        /// <returns>The created RenderTarget.</returns>
        public static RenderTarget Create(int width, int height)
        {
            if (Renderer.RendererApi == RendererApi.D3D11)
            {
                return new D3D11RenderTarget(width, height);
            }

            // TODO: Implement renderers

            throw new ReCraftedException("Invalid RendererApi!");
        }
    }
}