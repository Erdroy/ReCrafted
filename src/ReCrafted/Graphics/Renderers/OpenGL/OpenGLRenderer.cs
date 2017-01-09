// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

using System;
using OpenTK;
using OpenTK.Graphics;
using OpenTK.Graphics.OpenGL;
using OpenTK.Platform;
using ReCrafted.Core;
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

            // enable depth
            GL.Enable(EnableCap.CullFace);
            GL.CullFace(CullFaceMode.Back);
            GL.Enable(EnableCap.DepthTest);
            GL.Enable(EnableCap.DepthClamp);
            GL.DepthFunc(DepthFunction.Lequal);
            GL.DepthMask(true);
        }

        /// <summary>
        /// Ticks the renderer.
        /// </summary>
        public override void Tick()
        {
            Context.Update(WindowInfo);
        }

        /// <summary>
        /// Draws new frame.
        /// </summary>
        public override void Draw()
        {
            // TODO: Optimize color change
            var currentColor = Color.FromArgb(Camera.Current.BackgroundColor.A, Camera.Current.BackgroundColor.R, Camera.Current.BackgroundColor.G, Camera.Current.BackgroundColor.B);
            GL.ClearColor(currentColor);
            GL.ClearDepth(0.0f);
            GL.Clear(ClearBufferMask.ColorBufferBit | ClearBufferMask.DepthBufferBit);



            Context.SwapBuffers();
        }
        
        /// <summary>
        /// Reizes the targets.
        /// </summary>
        /// <param name="width">Width</param>
        /// <param name="height">Height</param>
        public override void Resize(int width, int height)
        {
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
        }

        /// <summary>
        /// Sets the depth test state.
        /// </summary>
        /// <param name="enabled">Disable or enable the depth test?</param>
        public override void SetDepthTestState(bool enabled)
        {
        }

        /// <summary>
        /// Set RenderTargets as the current frame output.
        /// </summary>
        /// <param name="renderTargets">The RenderTargets.</param>
        public override void SetRenderTargets(params RenderTarget[] renderTargets)
        {
        }

        /// <summary>
        /// Set RenderTargets as the current frame output.
        /// </summary>
        /// <param name="depthRenderTarget">The depth render target.</param>
        /// <param name="renderTargets">The RenderTargets.</param>
        public override void SetRenderTargetsDepth(RenderTarget depthRenderTarget, params RenderTarget[] renderTargets)
        {
        }

        /// <summary>
        /// Set the final render target.
        /// </summary>
        /// <param name="useDepthTest">Use DepthTest?</param>
        public override void SetFinalRenderTarget(bool useDepthTest)
        {
        }

        /// <summary>
        /// Render/Copy the given RenderTarget to the current set RenderTarget.
        /// </summary>
        /// <param name="renderTarget">The render target.</param>
        /// <param name="customShader">The custom shader for blit.</param>
        public override void Blit(RenderTarget renderTarget, Shader customShader = null)
        {
        }

        /// <summary>
        /// Clears depth.
        /// </summary>
        public override void ClearDepth()
        {
        }

        /// <summary>
        /// Dispatch ComputeShader.
        /// </summary>
        /// <param name="x">The X amount of threads.</param>
        /// <param name="y">The Y amount of threads.</param>
        /// <param name="z">The Z amount of threads.</param>
        public override void Dispatch(int x, int y, int z)
        {
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
        }

        /// <summary>
        /// Dispose the renderer.
        /// </summary>
        public override void Dispose()
        {
            Context?.Dispose();
        }

        public static GraphicsContext Context { get; private set; }

        public static IWindowInfo WindowInfo { get; private set; }
    }
}