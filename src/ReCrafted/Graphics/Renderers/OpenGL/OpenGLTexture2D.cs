// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

using System.Drawing;

namespace ReCrafted.Graphics.Renderers.OpenGLMesh
{
    /// <summary>
    /// OpenGLTexture2D class.
    /// </summary>
    // ReSharper disable once InconsistentNaming
    internal sealed class OpenGLTexture2D : Texture2D
    {
        /// <summary>
        /// Load texture from bitmap.
        /// </summary>
        /// <param name="bitmap">The bitmap.</param>
        /// <param name="genMips">Generate mipmaps for this texture?</param>
        /// <param name="maxMips">The maximal count of mipmaps.</param>
        protected override void Load(Bitmap bitmap, bool genMips, int maxMips)
        {
        }
        
        /// <summary>
        /// Dispose the texture.
        /// </summary>
        public override void Dispose()
        {
        }
    }
}