// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

using System;
using OpenTK.Graphics;
using OpenTK.Graphics.OpenGL;
using OpenTK.Platform;
using ReCrafted.Core;
using ReCrafted.Utilities;
using SharpDX;
using Color = System.Drawing.Color;

namespace ReCrafted.Graphics.Renderers.OpenGL
{
    /// <summary>
    /// OpenGLRenderer class.
    /// </summary>
    // ReSharper disable once InconsistentNaming
    internal sealed class OpenGLRenderer : Renderer
    {
        /// <summary>
        /// The OpenGLRenderer instance.
        /// </summary>
        public new static OpenGLRenderer Instance;
        
        private Shader _blitShader;

        private int _blitVao;

        /// <summary>
        /// Initializes the renderer.
        /// </summary>
        protected override void Init()
        {
            Instance = this;
            WindowInfo = OpenTK.Platform.Utilities.CreateWindowsWindowInfo(Game.Instance.Form.Handle);
            Context = new GraphicsContext(GraphicsMode.Default, WindowInfo, 3, 3, GraphicsContextFlags.Default);
            Context.MakeCurrent(WindowInfo);
            Context.LoadAll();

            DepthRenderTarget = RenderTarget.Create(Display.ClientWidth, Display.ClientHeight, RenderTarget.TextureFormat.Depth);

            GL.Viewport(0, 0, Display.ClientWidth, Display.ClientHeight);

            // enable
            GL.Enable(EnableCap.CullFace);
            GL.Enable(EnableCap.DepthTest);
            GL.Enable(EnableCap.DepthClamp);
            GL.Enable(EnableCap.StencilTest);
            CheckError();
            
            // setup depth
            GL.CullFace(CullFaceMode.Back);
            GL.DepthFunc(DepthFunction.Lequal);
            GL.DepthMask(true);
            
            CheckError();

            // setup framebuffer
            Framebuffer = GL.GenFramebuffer();

            // load blit shader
            _blitShader = Shader.FromFile("internal/Blit", false);
            _blitVao = GL.GenVertexArray();
        }

        /// <summary>
        /// Ticks the renderer.
        /// </summary>
        public override void Tick()
        {
        }

        /// <summary>
        /// Draws new frame.
        /// </summary>
        public override void Draw()
        {
            // reset
            GL.DepthMask(true);
            GL.BindFramebuffer(FramebufferTarget.Framebuffer, 0);
            CheckError();

            // TODO: Optimize color change
            var currentColor = Color.FromArgb(Camera.Current.BackgroundColor.A, Camera.Current.BackgroundColor.R, Camera.Current.BackgroundColor.G, Camera.Current.BackgroundColor.B);
            GL.ClearColor(currentColor);
            GL.Clear(ClearBufferMask.ColorBufferBit | ClearBufferMask.DepthBufferBit);
            CheckError();

            Rendering.Draw();
            CheckError();

            Context.SwapBuffers();
            CheckError();
        }
        
        /// <summary>
        /// Reizes the targets.
        /// </summary>
        /// <param name="width">Width</param>
        /// <param name="height">Height</param>
        public override void Resize(int width, int height)
        {
            WindowInfo = OpenTK.Platform.Utilities.CreateWindowsWindowInfo(Game.Instance.Form.Handle);
            Context.Update(WindowInfo);
            Context.MakeCurrent(WindowInfo);
            SetViewportSize(width, height);
            
            DepthRenderTarget.Resize(width, height);

            CheckError();
        }

        /// <summary>
        /// Sets rasterizer state.
        /// </summary>
        /// <param name="state">The state.</param>
        public override void SetRasterizerState(RasterizerStates state)
        {
        }

        /// <summary>
        /// Set the viewport size.
        /// </summary>
        /// <param name="width">The width.</param>
        /// <param name="height">The height.</param>
        public override void SetViewportSize(int width, int height)
        {
            GL.Viewport(0, 0, width, height);
            CheckError();
        }

        /// <summary>
        /// Sets the depth test state.
        /// </summary>
        /// <param name="enabled">Disable or enable the depth test?</param>
        public override void SetDepthTestState(bool enabled)
        {
            GL.DepthMask(enabled);
            CheckError();
        }

        /// <summary>
        /// Set RenderTargets as the current frame output.
        /// </summary>
        /// <param name="renderTargets">The RenderTargets.</param>
        public override void SetRenderTargets(params RenderTarget[] renderTargets)
        {
            var index = 0;

            GL.BindFramebuffer(FramebufferTarget.Framebuffer, Framebuffer);
            GL.FramebufferRenderbuffer(FramebufferTarget.Framebuffer, FramebufferAttachment.DepthAttachment, RenderbufferTarget.Renderbuffer, 0);

            var drawbuffers = new DrawBuffersEnum[renderTargets.Length];
            foreach (var renderTarget in renderTargets)
            {
                var openglrt = (OpenGLRenderTarget)renderTarget;

                GL.FramebufferRenderbuffer(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment0 + index, RenderbufferTarget.Renderbuffer, openglrt.Renderbuffer);
                GL.FramebufferTexture2D(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment0 + index, TextureTarget.Texture2D, openglrt.Texture, 0);

                drawbuffers[index] = DrawBuffersEnum.ColorAttachment0 + index;

                index++;
            }
            GL.DrawBuffers(drawbuffers.Length, drawbuffers);

            CheckError();
        }

        /// <summary>
        /// Set RenderTargets as the current frame output.
        /// </summary>
        /// <param name="depthRenderTarget">The depth render target.</param>
        /// <param name="renderTargets">The RenderTargets.</param>
        public override void SetRenderTargetsDepth(RenderTarget depthRenderTarget, params RenderTarget[] renderTargets)
        {
            var index = 0;
            
            var depthrt = ((OpenGLRenderTarget)depthRenderTarget);
            GL.BindFramebuffer(FramebufferTarget.Framebuffer, Framebuffer);
            GL.FramebufferRenderbuffer(FramebufferTarget.Framebuffer, FramebufferAttachment.DepthAttachment, RenderbufferTarget.Renderbuffer, depthrt.Renderbuffer);
            GL.FramebufferTexture2D(FramebufferTarget.Framebuffer, FramebufferAttachment.DepthAttachment, TextureTarget.Texture2D, depthrt.Texture, 0);

            var drawbuffers = new DrawBuffersEnum[renderTargets.Length];
            foreach (var renderTarget in renderTargets)
            {
                var openglrt = (OpenGLRenderTarget) renderTarget;

                GL.FramebufferRenderbuffer(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment0 + index,
                    RenderbufferTarget.Renderbuffer, openglrt.Renderbuffer);
                GL.FramebufferTexture2D(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment0 + index,
                    TextureTarget.Texture2D, openglrt.Texture, 0);

                drawbuffers[index] = DrawBuffersEnum.ColorAttachment0 + index;

                index++;
            }

            GL.DrawBuffers(drawbuffers.Length, drawbuffers);
            
            CheckError();
        }

        /// <summary>
        /// Set the final render target.
        /// </summary>
        /// <param name="useDepthTest">Use DepthTest?</param>
        public override void SetFinalRenderTarget(bool useDepthTest)
        {
            GL.BindFramebuffer(FramebufferTarget.Framebuffer, 0);
            GL.BindTexture(TextureTarget.Texture2D, 0);
            GL.DrawBuffer(DrawBufferMode.Back);

            SetDepthTestState(useDepthTest);
            CheckError();
        }

        /// <summary>
        /// Render/Copy the given RenderTarget to the current set RenderTarget.
        /// </summary>
        /// <param name="renderTarget">The render target.</param>
        /// <param name="customShader">The custom shader for blit.</param>
        public override void Blit(RenderTarget renderTarget, Shader customShader = null)
        {
            GL.DepthMask(false);

            var glShader = (OpenGLShader)_blitShader;
            glShader.Apply();
            glShader.SetRenderTexture(ShaderType.PS, 0, renderTarget);
            CheckError();

            GL.BindVertexArray(_blitVao);
            GL.DrawArrays(OpenTK.Graphics.OpenGL.PrimitiveType.TriangleStrip, 0, 4);
            CheckError();
        }

        /// <summary>
        /// Clears depth.
        /// </summary>
        public override void ClearDepth()
        {
            // just clear the depth
            GL.Clear(ClearBufferMask.DepthBufferBit);
            CheckError();
        }

        /// <summary>
        /// Changes face culling
        /// </summary>
        public override void FaceCulling(bool front, bool back)
        {
            if(back && !front)
                GL.CullFace(CullFaceMode.Back);
            else if(front && !back)
                GL.CullFace(CullFaceMode.Front);
            else
                GL.CullFace(CullFaceMode.FrontAndBack);
        }

        /// <summary>
        /// Dispatch ComputeShader.
        /// </summary>
        /// <param name="x">The X amount of threads.</param>
        /// <param name="y">The Y amount of threads.</param>
        /// <param name="z">The Z amount of threads.</param>
        public override void Dispatch(int x, int y, int z)
        {
            // ?
        }

        /// <summary>
        /// Gets all RenderTargets from the current frame output.
        /// </summary>
        /// <returns>The RenderTargets.</returns>
        public override RenderTarget[] GetRenderTargets()
        {
            throw new NotImplementedException("GetRenderTargets() is not implemented currently!");
        }
        
        /// <summary>
        /// Draw bounding box.
        /// </summary>
        /// <param name="bounds">The bouding box.</param>
        public override void DrawBoundingBox(BoundingBox bounds)
        {
            throw new NotImplementedException("DrawBoundingBox() is not implemented in OpenGL renderer, switch to D3D11.");
        }

        /// <summary>
        /// Dispose the renderer.
        /// </summary>
        public override void Dispose()
        {
            Context?.Dispose();
            _blitShader?.Dispose();

            if (Framebuffer >= 0)
                GL.DeleteFramebuffer(Framebuffer);
        }

        /// <summary>
        /// Check errors
        /// </summary>
        public static void CheckError()
        {
            var error = GL.GetError();

            if (error != ErrorCode.NoError)
                throw new ReCraftedException("OpenGL Error, errorcode: " + error);
        }

        public static GraphicsContext Context { get; private set; }

        public static IWindowInfo WindowInfo { get; private set; }

        public int Framebuffer { get; private set; } = -1;
    }
}