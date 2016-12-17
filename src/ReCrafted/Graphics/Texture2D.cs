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
        /// <param name="genMips">Generate mipmaps for this texture?</param>
        /// <param name="maxMips">The maximal count of mipmaps.</param>
        protected abstract void Load(Bitmap bitmap, bool genMips, int maxMips);
        
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
        /// <param name="generateMips">Generate mipmaps for this texture?</param>
        /// <param name="maxMips">The maximal count of mipmaps.</param>
        /// <returns>The loaded texture, null when failed(exception will be thrown).</returns>
        public static Texture2D FromFile(string file, bool generateMips = false, int maxMips = 8)
        {
            switch (Renderer.RendererApi)
            {
                case RendererApi.D3D11:
                {
                    var texture = new D3D11Texture2D();
                    texture.Load(new Bitmap(file), generateMips, maxMips);
                    return texture;
                }
            }

            // TODO: Implement renderers

            throw new ReCraftedException("Invalid RendererApi!");
        }
    }
}