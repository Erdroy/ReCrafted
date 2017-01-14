// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

using System.Drawing;
using System.Drawing.Imaging;
using OpenTK.Graphics.OpenGL;

namespace ReCrafted.Graphics.Renderers.OpenGL
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
            int texture;
            GL.GenTextures(1, out texture);
            GL.BindTexture(TextureTarget.Texture2D, texture);

            var data = bitmap.LockBits(new Rectangle(0, 0, bitmap.Width, bitmap.Height), ImageLockMode.ReadOnly, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
            GL.TexImage2D(TextureTarget.Texture2D, 0, PixelInternalFormat.Rgba, data.Width, data.Height, 0,  OpenTK.Graphics.OpenGL.PixelFormat.Bgra, PixelType.UnsignedByte, data.Scan0);
            bitmap.UnlockBits(data);
            
            GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMinFilter, (int)TextureMinFilter.Linear);
            GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMagFilter, (int)TextureMagFilter.Linear);
            GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureWrapS, (int)TextureWrapMode.Repeat);
            GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureWrapT, (int)TextureWrapMode.Repeat);

            Texture = texture;
            GL.BindTexture(TextureTarget.Texture2D, 0);

            OpenGLRenderer.CheckError();
        }

        /// <summary>
        /// Dispose the texture.
        /// </summary>
        public override void Dispose()
        {
            if(Texture >= 0)
                GL.DeleteTexture(Texture);
        }

        public int Texture { get; private set; } = -1;
    }
}