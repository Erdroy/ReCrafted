// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

using System;
using System.Collections.Generic;
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

        private readonly List<DrawBuffersEnum> _drawBuffers = new List<DrawBuffersEnum>();
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

            GL.Viewport(0, 0, Display.ClientWidth, Display.ClientHeight);

            // enable depth
            GL.Enable(EnableCap.CullFace);
            GL.CullFace(CullFaceMode.Back);
            GL.Enable(EnableCap.DepthTest);
            GL.Enable(EnableCap.DepthClamp);
            GL.DepthFunc(DepthFunction.Lequal);
            GL.DepthMask(true);
            
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
            _drawBuffers.Clear();
            switch (renderTargets.Length)
            {
                case 1:// TODO BIND FRAMEBUFFER <-------------------------------------------------------------
                    GL.BindFramebuffer(FramebufferTarget.Framebuffer, ((OpenGLRenderTarget)renderTargets[0]).Framebuffer);

                    GL.FramebufferTexture(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment0, ((OpenGLRenderTarget)renderTargets[0]).Framebuffer, 0);

                    _drawBuffers.Add(DrawBuffersEnum.ColorAttachment0);
                    break;
                case 2:
                    GL.BindFramebuffer(FramebufferTarget.Framebuffer, ((OpenGLRenderTarget)renderTargets[0]).Framebuffer);
                    GL.BindFramebuffer(FramebufferTarget.Framebuffer, ((OpenGLRenderTarget)renderTargets[1]).Framebuffer);

                    GL.FramebufferTexture(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment0, ((OpenGLRenderTarget)renderTargets[0]).Framebuffer, 0);
                    GL.FramebufferTexture(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment1, ((OpenGLRenderTarget)renderTargets[1]).Framebuffer, 0);

                    _drawBuffers.Add(DrawBuffersEnum.ColorAttachment0);
                    _drawBuffers.Add(DrawBuffersEnum.ColorAttachment1);
                    break;
                case 3:
                    GL.BindFramebuffer(FramebufferTarget.Framebuffer, ((OpenGLRenderTarget)renderTargets[0]).Framebuffer);
                    GL.BindFramebuffer(FramebufferTarget.Framebuffer, ((OpenGLRenderTarget)renderTargets[1]).Framebuffer);
                    GL.BindFramebuffer(FramebufferTarget.Framebuffer, ((OpenGLRenderTarget)renderTargets[2]).Framebuffer);

                    GL.FramebufferTexture(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment0, ((OpenGLRenderTarget)renderTargets[0]).Framebuffer, 0);
                    GL.FramebufferTexture(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment1, ((OpenGLRenderTarget)renderTargets[1]).Framebuffer, 0);
                    GL.FramebufferTexture(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment2, ((OpenGLRenderTarget)renderTargets[2]).Framebuffer, 0);

                    _drawBuffers.Add(DrawBuffersEnum.ColorAttachment0);
                    _drawBuffers.Add(DrawBuffersEnum.ColorAttachment1);
                    _drawBuffers.Add(DrawBuffersEnum.ColorAttachment2);
                    break;
                case 4:
                    GL.FramebufferTexture(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment0, ((OpenGLRenderTarget)renderTargets[0]).Framebuffer, 0);
                    GL.FramebufferTexture(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment1, ((OpenGLRenderTarget)renderTargets[1]).Framebuffer, 0);
                    GL.FramebufferTexture(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment2, ((OpenGLRenderTarget)renderTargets[2]).Framebuffer, 0);
                    GL.FramebufferTexture(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment3, ((OpenGLRenderTarget)renderTargets[3]).Framebuffer, 0);

                    _drawBuffers.Add(DrawBuffersEnum.ColorAttachment0);
                    _drawBuffers.Add(DrawBuffersEnum.ColorAttachment1);
                    _drawBuffers.Add(DrawBuffersEnum.ColorAttachment2);
                    _drawBuffers.Add(DrawBuffersEnum.ColorAttachment3);
                    break;
                case 5:
                    GL.FramebufferTexture(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment0, ((OpenGLRenderTarget)renderTargets[0]).Framebuffer, 0);
                    GL.FramebufferTexture(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment1, ((OpenGLRenderTarget)renderTargets[1]).Framebuffer, 0);
                    GL.FramebufferTexture(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment2, ((OpenGLRenderTarget)renderTargets[2]).Framebuffer, 0);
                    GL.FramebufferTexture(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment3, ((OpenGLRenderTarget)renderTargets[3]).Framebuffer, 0);
                    GL.FramebufferTexture(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment4, ((OpenGLRenderTarget)renderTargets[4]).Framebuffer, 0);

                    _drawBuffers.Add(DrawBuffersEnum.ColorAttachment0);
                    _drawBuffers.Add(DrawBuffersEnum.ColorAttachment1);
                    _drawBuffers.Add(DrawBuffersEnum.ColorAttachment2);
                    _drawBuffers.Add(DrawBuffersEnum.ColorAttachment3);
                    _drawBuffers.Add(DrawBuffersEnum.ColorAttachment4);
                    break;
                case 6:
                    GL.FramebufferTexture(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment0, ((OpenGLRenderTarget)renderTargets[0]).Framebuffer, 0);
                    GL.FramebufferTexture(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment1, ((OpenGLRenderTarget)renderTargets[1]).Framebuffer, 0);
                    GL.FramebufferTexture(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment2, ((OpenGLRenderTarget)renderTargets[2]).Framebuffer, 0);
                    GL.FramebufferTexture(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment3, ((OpenGLRenderTarget)renderTargets[3]).Framebuffer, 0);
                    GL.FramebufferTexture(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment4, ((OpenGLRenderTarget)renderTargets[4]).Framebuffer, 0);
                    GL.FramebufferTexture(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment5, ((OpenGLRenderTarget)renderTargets[5]).Framebuffer, 0);

                    _drawBuffers.Add(DrawBuffersEnum.ColorAttachment0);
                    _drawBuffers.Add(DrawBuffersEnum.ColorAttachment1);
                    _drawBuffers.Add(DrawBuffersEnum.ColorAttachment2);
                    _drawBuffers.Add(DrawBuffersEnum.ColorAttachment3);
                    _drawBuffers.Add(DrawBuffersEnum.ColorAttachment4);
                    _drawBuffers.Add(DrawBuffersEnum.ColorAttachment5);
                    break;
                default:
                    throw new ReCraftedException("OpenGL renderer supports only max. 6 render targets.");
            }
            
            GL.DrawBuffers(_drawBuffers.Count, _drawBuffers.ToArray());
            CheckError();
        }

        /// <summary>
        /// Set RenderTargets as the current frame output.
        /// </summary>
        /// <param name="depthRenderTarget">The depth render target.</param>
        /// <param name="renderTargets">The RenderTargets.</param>
        public override void SetRenderTargetsDepth(RenderTarget depthRenderTarget, params RenderTarget[] renderTargets)
        {
            _drawBuffers.Clear();
            switch (renderTargets.Length)
            {
                case 1:
                    GL.FramebufferTexture(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment0, ((OpenGLRenderTarget)renderTargets[0]).Framebuffer, 0);

                    _drawBuffers.Add(DrawBuffersEnum.ColorAttachment0);
                    break;
                case 2:
                    GL.FramebufferTexture(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment0, ((OpenGLRenderTarget)renderTargets[0]).Framebuffer, 0);
                    GL.FramebufferTexture(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment1, ((OpenGLRenderTarget)renderTargets[1]).Framebuffer, 0);

                    _drawBuffers.Add(DrawBuffersEnum.ColorAttachment0);
                    _drawBuffers.Add(DrawBuffersEnum.ColorAttachment1);
                    break;
                case 3:
                    GL.FramebufferTexture(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment0, ((OpenGLRenderTarget)renderTargets[0]).Framebuffer, 0);
                    GL.FramebufferTexture(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment1, ((OpenGLRenderTarget)renderTargets[1]).Framebuffer, 0);
                    GL.FramebufferTexture(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment2, ((OpenGLRenderTarget)renderTargets[2]).Framebuffer, 0);

                    _drawBuffers.Add(DrawBuffersEnum.ColorAttachment0);
                    _drawBuffers.Add(DrawBuffersEnum.ColorAttachment1);
                    _drawBuffers.Add(DrawBuffersEnum.ColorAttachment2);
                    break;
                case 4:
                    GL.FramebufferTexture(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment0, ((OpenGLRenderTarget)renderTargets[0]).Framebuffer, 0);
                    GL.FramebufferTexture(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment1, ((OpenGLRenderTarget)renderTargets[1]).Framebuffer, 0);
                    GL.FramebufferTexture(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment2, ((OpenGLRenderTarget)renderTargets[2]).Framebuffer, 0);
                    GL.FramebufferTexture(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment3, ((OpenGLRenderTarget)renderTargets[3]).Framebuffer, 0);

                    _drawBuffers.Add(DrawBuffersEnum.ColorAttachment0);
                    _drawBuffers.Add(DrawBuffersEnum.ColorAttachment1);
                    _drawBuffers.Add(DrawBuffersEnum.ColorAttachment2);
                    _drawBuffers.Add(DrawBuffersEnum.ColorAttachment3);
                    break;
                case 5:
                    GL.FramebufferTexture(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment0, ((OpenGLRenderTarget)renderTargets[0]).Framebuffer, 0);
                    GL.FramebufferTexture(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment1, ((OpenGLRenderTarget)renderTargets[1]).Framebuffer, 0);
                    GL.FramebufferTexture(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment2, ((OpenGLRenderTarget)renderTargets[2]).Framebuffer, 0);
                    GL.FramebufferTexture(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment3, ((OpenGLRenderTarget)renderTargets[3]).Framebuffer, 0);
                    GL.FramebufferTexture(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment4, ((OpenGLRenderTarget)renderTargets[4]).Framebuffer, 0);

                    _drawBuffers.Add(DrawBuffersEnum.ColorAttachment0);
                    _drawBuffers.Add(DrawBuffersEnum.ColorAttachment1);
                    _drawBuffers.Add(DrawBuffersEnum.ColorAttachment2);
                    _drawBuffers.Add(DrawBuffersEnum.ColorAttachment3);
                    _drawBuffers.Add(DrawBuffersEnum.ColorAttachment4);
                    break;
                case 6:
                    GL.FramebufferTexture(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment0, ((OpenGLRenderTarget)renderTargets[0]).Framebuffer, 0);
                    GL.FramebufferTexture(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment1, ((OpenGLRenderTarget)renderTargets[1]).Framebuffer, 0);
                    GL.FramebufferTexture(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment2, ((OpenGLRenderTarget)renderTargets[2]).Framebuffer, 0);
                    GL.FramebufferTexture(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment3, ((OpenGLRenderTarget)renderTargets[3]).Framebuffer, 0);
                    GL.FramebufferTexture(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment4, ((OpenGLRenderTarget)renderTargets[4]).Framebuffer, 0);
                    GL.FramebufferTexture(FramebufferTarget.Framebuffer, FramebufferAttachment.ColorAttachment5, ((OpenGLRenderTarget)renderTargets[5]).Framebuffer, 0);

                    _drawBuffers.Add(DrawBuffersEnum.ColorAttachment0);
                    _drawBuffers.Add(DrawBuffersEnum.ColorAttachment1);
                    _drawBuffers.Add(DrawBuffersEnum.ColorAttachment2);
                    _drawBuffers.Add(DrawBuffersEnum.ColorAttachment3);
                    _drawBuffers.Add(DrawBuffersEnum.ColorAttachment4);
                    _drawBuffers.Add(DrawBuffersEnum.ColorAttachment5);
                    break;
                default:
                    throw new ReCraftedException("OpenGL renderer supports only max. 6 render targets.");
            }

            GL.DrawBuffers(_drawBuffers.Count, _drawBuffers.ToArray());
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
        }

        /// <summary>
        /// Check errors
        /// </summary>
        public static void CheckError()
        {
            var error = GL.GetError();

            if (error != ErrorCode.NoError)
                throw new ReCraftedException("OpenGL Error");
        }

        public static GraphicsContext Context { get; private set; }

        public static IWindowInfo WindowInfo { get; private set; }
    }
}