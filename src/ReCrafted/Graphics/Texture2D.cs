// ReCrafted © 2016 Damian 'Erdroy' Korczowski

using System;
using System.Drawing;
using ReCrafted.Graphics.Renderers.D3D11;
using ReCrafted.Utilities;

namespace ReCrafted.Graphics
{
    /// <summary>
    /// Texture2D class.
    /// </summary>
    public abstract class Texture2D : IDisposable
    {
        // lock from creating
        protected Texture2D() { }

        /// <summary>
        /// Load texture from bitmap.
        /// </summary>
        /// <param name="bitmap">The bitmap.</param>
        protected abstract void Load(Bitmap bitmap);
        
        /// <summary>
        /// Dispose the texture.
        /// </summary>
        public abstract void Dispose();
        
        /// <summary>
        /// Create new texture using given width and height.
        /// </summary>
        /// <param name="width">The width.</param>
        /// <param name="height">The height.</param>
        /// <returns>Created texture, null when failed(exception will be thrown).</returns>
        public static Texture2D New(int width, int height)
        {
            throw new ReCraftedException("Not implemented!"); // TODO: Implement Texture2D::New
        }

        /// <summary>
        /// Load 2d texture from file.
        /// </summary>
        /// <param name="file">The file path.</param>
        /// <returns>The loaded texture, null when failed(exception will be thrown).</returns>
        public static Texture2D FromFile(string file)
        {
            if (Renderer.RendererApi == RendererApi.D3D11)
            {
                var d3D11Texture = new D3D11Texture2D();
                d3D11Texture.Load(new Bitmap(file));
                return d3D11Texture;
            }

            // TODO: Implement renderers

            throw new ReCraftedException("Invalid RendererApi!");
        }
    }
}