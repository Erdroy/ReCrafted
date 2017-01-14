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
            if (_format == TextureFormat.Depth)
            {
                GL.BindFramebuffer(FramebufferTarget.Framebuffer, ((OpenGLRenderer)Renderer.Instance).Framebuffer);
                GL.FramebufferRenderbuffer(FramebufferTarget.Framebuffer, FramebufferAttachment.DepthAttachment, RenderbufferTarget.Renderbuffer, Renderbuffer);
                GL.FramebufferTexture2D(FramebufferTarget.Framebuffer, FramebufferAttachment.DepthAttachment, TextureTarget.Texture2D, Texture, 0);

                OpenGLRenderer.Instance.ClearDepth();
                GL.BindFramebuffer(FramebufferTarget.Framebuffer, 0);
                return;
            }

            var vec = color.ToVector4();
            GL.BindFramebuffer(FramebufferTarget.Framebuffer, ((OpenGLRenderer)Renderer.Instance).Framebuffer);
            GL.FramebufferRenderbuffer(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment0, RenderbufferTarget.Renderbuffer, Renderbuffer);
            GL.FramebufferTexture2D(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment0, TextureTarget.Texture2D, Texture, 0);
            GL.ClearColor(vec.X, vec.Y, vec.Z, vec.W);
            GL.Clear(ClearBufferMask.ColorBufferBit);

            GL.BindFramebuffer(FramebufferTarget.Framebuffer, 0);
            OpenGLRenderer.CheckError();
        }

        /// <summary>
        /// Dispose the RenderTarget.
        /// </summary>
        public override void Dispose()
        {
            if (Renderbuffer >= 0)
                GL.DeleteRenderbuffer(Renderbuffer);

            if (Texture >= 0)
                GL.DeleteTexture(Texture);

            GL.BindFramebuffer(FramebufferTarget.Framebuffer, 0);
            OpenGLRenderer.CheckError();
        }

        // private
        private void Internal_Create(int width, int height, bool uav)
        {
            if (Renderbuffer >= 0)
            {
                Dispose();
            }

            var format = PixelInternalFormat.Rgba;
            var pixelformat = PixelFormat.Rgba;
            var pixeltype = PixelType.Float;

            var bufferFormat = RenderbufferStorage.Rgb8ui;

            switch (_format)
            {
                case TextureFormat.Depth:
                    format = PixelInternalFormat.DepthStencil;
                    break;
                case TextureFormat.R16_Float:
                    format = PixelInternalFormat.R16f;
                    bufferFormat = RenderbufferStorage.R16f;
                    pixelformat = PixelFormat.Red;
                    pixeltype = PixelType.Float;
                    break;
                case TextureFormat.R32_Float:
                    format = PixelInternalFormat.R32f;
                    bufferFormat = RenderbufferStorage.R32f;
                    pixelformat = PixelFormat.Red;
                    pixeltype = PixelType.Float;
                    break;
                case TextureFormat.RG32_Float:
                    format = PixelInternalFormat.Rg32f;
                    bufferFormat = RenderbufferStorage.Rg32f;
                    pixelformat = PixelFormat.Rg;
                    pixeltype = PixelType.Float;
                    break;
                case TextureFormat.RGB32_Float:
                    format = PixelInternalFormat.Rgb32f;
                    bufferFormat = RenderbufferStorage.Rgb32f;
                    pixelformat = PixelFormat.Rgb;
                    pixeltype = PixelType.Float;
                    break;
                case TextureFormat.RGBA32_Float:
                    format = PixelInternalFormat.Rgba32f;
                    bufferFormat = RenderbufferStorage.Rgba32f;
                    pixelformat = PixelFormat.Rgba;
                    pixeltype = PixelType.Float;
                    break;
                case TextureFormat.RGBA8_UNorm:
                    format = PixelInternalFormat.Rgba8;
                    bufferFormat = RenderbufferStorage.Rgba8ui;
                    pixelformat = PixelFormat.Rgba;
                    pixeltype = PixelType.UnsignedInt;
                    break;
            }

            // generate resources
            Renderbuffer = GL.GenRenderbuffer();
            Texture = GL.GenTexture();

            // bind resources
            GL.BindRenderbuffer(RenderbufferTarget.Renderbuffer, Renderbuffer);
            GL.BindTexture(TextureTarget.Texture2D, Texture);

            if (format == PixelInternalFormat.DepthStencil)
            {
                // TODO: depth renderbuffer
                GL.RenderbufferStorage(RenderbufferTarget.Renderbuffer, RenderbufferStorage.DepthComponent24, width, height);

                GL.TexImage2D(TextureTarget.Texture2D, 0, PixelInternalFormat.DepthComponent24, width, height, 0, PixelFormat.DepthComponent, PixelType.Float, IntPtr.Zero);
                GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMinFilter, (int)TextureMinFilter.Linear);
                GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMagFilter, (int)TextureMagFilter.Linear);
            }
            else
            {
                GL.RenderbufferStorage(RenderbufferTarget.Renderbuffer, bufferFormat, width, height);
                GL.TexImage2D(TextureTarget.Texture2D, 0, format, width, height, 0, pixelformat, pixeltype, IntPtr.Zero);
                GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMinFilter, (int)TextureMinFilter.Linear);
                GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMagFilter, (int)TextureMagFilter.Linear);
            }

            // clear
            GL.BindRenderbuffer(RenderbufferTarget.Renderbuffer, 0);
            GL.BindTexture(TextureTarget.Texture2D, 0);
        }

        /// <summary>
        /// The framebuffer pointer
        /// </summary>
        public int Renderbuffer { get; private set; } = -1;

        /// <summary>
        /// The framebuffer's texture pointer
        /// </summary>
        public int Texture { get; private set; } = -1;
    }
}

