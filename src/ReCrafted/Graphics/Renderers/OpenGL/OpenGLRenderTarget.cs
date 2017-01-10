// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

using System;
using OpenTK.Graphics.OpenGL;
using SharpDX;

namespace ReCrafted.Graphics.Renderers.OpenGL
{
    /// <summary>
    /// OpenGLRenderTarget class.
    /// </summary>
    // ReSharper disable once InconsistentNaming
    public sealed class OpenGLRenderTarget : RenderTarget
    {
        private bool _isUav;
        private TextureFormat _format;

        /// <summary>
        /// OpenGLRenderTarget constructor.
        /// </summary>
        public OpenGLRenderTarget(int width, int height, TextureFormat format, bool uav)
        {
            _format = format;
            _isUav = uav;
            Internal_Create(width, height, _isUav);
        }

        /// <summary>
        /// Resize the render target.
        /// </summary>
        public override void Resize(int width, int height)
        {
            Internal_Create(width, height, _isUav);
        }

        /// <summary>
        /// Clear the render target using given color.
        /// </summary>
        /// <param name="color">The color.</param>
        public override void Clear(Color color)
        {
            var vec = color.ToVector4();
            GL.BindFramebuffer(FramebufferTarget.Framebuffer, Framebuffer);
            GL.ClearColor(vec.X, vec.Y, vec.Z, vec.W);
            GL.Clear(ClearBufferMask.ColorBufferBit);
            GL.BindFramebuffer(FramebufferTarget.Framebuffer, 0);
        }

        /// <summary>
        /// Dispose the RenderTarget.
        /// </summary>
        public override void Dispose()
        {
            if (Framebuffer >= 0)
                GL.DeleteFramebuffer(Framebuffer);

            if (Texture >= 0)
                GL.DeleteTexture(Texture);
        }

        // private
        private void Internal_Create(int width, int height, bool uav)
        {
            Dispose();

            Framebuffer = GL.GenFramebuffer();
            GL.BindFramebuffer(FramebufferTarget.Framebuffer, Framebuffer);

            var format = PixelInternalFormat.Rgba;
            var format1 = PixelFormat.Rgba;
            var type = PixelType.UnsignedByte;

            switch (_format)
            {
                case TextureFormat.Depth:
                    format = PixelInternalFormat.DepthStencil;
                    break;
                case TextureFormat.R16_Float:
                    format = PixelInternalFormat.R16f;
                    format1 = PixelFormat.Red;
                    type = PixelType.Float;
                    break;
                case TextureFormat.R32_Float:
                    format = PixelInternalFormat.R32f;
                    format1 = PixelFormat.Red;
                    type = PixelType.Float;
                    break;
                case TextureFormat.RG32_Float:
                    format = PixelInternalFormat.Rg32f;
                    format1 = PixelFormat.Rg;
                    type = PixelType.Float;
                    break;
                case TextureFormat.RGB32_Float:
                    format = PixelInternalFormat.Rgb32f;
                    format1 = PixelFormat.Rgb;
                    type = PixelType.Float;
                    break;
                case TextureFormat.RGBA32_Float:
                    format = PixelInternalFormat.Rgba32f;
                    format1 = PixelFormat.Rgba;
                    type = PixelType.Float;
                    break;
                case TextureFormat.RGBA8_UNorm:
                    format = PixelInternalFormat.Rgba8;
                    format1 = PixelFormat.Rgba;
                    type = PixelType.UnsignedInt;
                    break;
            }

            Texture = GL.GenTexture();
            GL.BindTexture(TextureTarget.Texture2D, Texture);

            if (format == PixelInternalFormat.DepthStencil)
                GL.TexImage2D(TextureTarget.Texture2D, 0, PixelInternalFormat.DepthComponent, width, height, 0, PixelFormat.DepthComponent, PixelType.UnsignedInt, IntPtr.Zero);
            else
                GL.TexImage2D(TextureTarget.Texture2D, 0, format, width, height, 0, format1, type, IntPtr.Zero);

            GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMinFilter, (int)TextureMinFilter.Nearest);
            GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMagFilter, (int)TextureMinFilter.Nearest);

            GL.FramebufferTexture(FramebufferTarget.Framebuffer,
                format == PixelInternalFormat.DepthStencil
                    ? FramebufferAttachment.DepthAttachment
                    : FramebufferAttachment.ColorAttachment0, Texture, 0);

            // clear
            GL.BindTexture(TextureTarget.Texture2D, 0);
            GL.BindFramebuffer(FramebufferTarget.Framebuffer, 0);

            /*
             * https://www.opengl.org/discussion_boards/showthread.php/198528-OpenTK-Depth-Buffer-as-texture-problems
             * http://stackoverflow.com/questions/24109208/opengl-framebuffer-depth-buffer-not-working
             ////Create color attachment texture
            GL.FramebufferTexture(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment0, textureColorBuffer, 0);
            GL.FramebufferTexture(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment1, normalBuffer, 0);
            GL.FramebufferTexture(FramebufferTarget.Framebuffer, FramebufferAttachment.DepthAttachment, depthBuffer, 0);

            DrawBuffersEnum[] bufs = new DrawBuffersEnum[2] { (DrawBuffersEnum)FramebufferAttachment.ColorAttachment0, (DrawBuffersEnum)FramebufferAttachment.ColorAttachment1 };
            GL.DrawBuffers(bufs.Length, bufs);

            GL.BindFramebuffer(FramebufferTarget.Framebuffer, 0);
             */
        }

        /// <summary>
        /// The framebuffer pointer
        /// </summary>
        public int Framebuffer { get; private set; } = -1;

        /// <summary>
        /// The framebuffer's texture pointer
        /// </summary>
        public int Texture { get; private set; } = -1;
    }
}
