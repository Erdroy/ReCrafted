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
        /// <param name="samplerType">The sampler type of the texture.</param>
        protected override void Load(Bitmap bitmap, bool genMips, int maxMips, Sampler.Type samplerType)
        {
            int texture;
            GL.GenTextures(1, out texture);
            GL.BindTexture(TextureTarget.Texture2D, texture);

            var data = bitmap.LockBits(new Rectangle(0, 0, bitmap.Width, bitmap.Height), ImageLockMode.ReadOnly, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
            GL.TexImage2D(TextureTarget.Texture2D, 0, PixelInternalFormat.Rgba, data.Width, data.Height, 0,  OpenTK.Graphics.OpenGL.PixelFormat.Bgra, PixelType.UnsignedByte, data.Scan0);
            bitmap.UnlockBits(data);
            
            if (genMips)
            {
                GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMaxLevel, maxMips);
                GL.GenerateMipmap(GenerateMipmapTarget.Texture2D);
            }

            // POINT
            if (samplerType == Sampler.Type.PointClamped 
                || samplerType == Sampler.Type.PointMirror 
                || samplerType == Sampler.Type.PointWrap)
            {
                GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMinFilter, (int)TextureMinFilter.NearestMipmapLinear);
                GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMagFilter, (int)TextureMagFilter.Nearest);
            }

            // LINEAR
            if (samplerType == Sampler.Type.LinearClamped 
                || samplerType == Sampler.Type.LinearMirror 
                || samplerType == Sampler.Type.LinearWrap)
            {
                GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMinFilter, (int)TextureMinFilter.LinearMipmapNearest);
                GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMagFilter, (int)TextureMagFilter.Linear);
            }

            // ANIZO
            if (samplerType == Sampler.Type.AnisoClamped
                || samplerType == Sampler.Type.AnisoMirror 
                || samplerType == Sampler.Type.AnisoWrap)
            {
                GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMinFilter, (int)TextureMinFilter.LinearMipmapNearest);
                GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMagFilter, (int)TextureMagFilter.Nearest);
            }

            // CLAMPED
            if (samplerType == Sampler.Type.PointClamped
                || samplerType == Sampler.Type.LinearClamped
                || samplerType == Sampler.Type.AnisoClamped)
            {
                GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureWrapS, (int)TextureWrapMode.Clamp);
                GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureWrapT, (int)TextureWrapMode.Clamp);
            }

            // MIRROR
            if (samplerType == Sampler.Type.LinearMirror
                || samplerType == Sampler.Type.AnisoMirror
                || samplerType == Sampler.Type.PointMirror)
            {
                GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureWrapS, (int)TextureWrapMode.MirroredRepeat);
                GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureWrapT, (int)TextureWrapMode.MirroredRepeat);
            }

            // WRAP
            if (samplerType == Sampler.Type.AnisoWrap
                || samplerType == Sampler.Type.LinearWrap
                || samplerType == Sampler.Type.PointWrap)
            {
                GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureWrapS, (int)TextureWrapMode.ClampToEdge);
                GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureWrapT, (int)TextureWrapMode.ClampToEdge);
            }

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