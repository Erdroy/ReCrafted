// ReCrafted © 2016 Damian 'Erdroy' Korczowski

using System;
using ReCrafted.Core;
using ReCrafted.Graphics.Renderers.D3D11;
using ReCrafted.Utilities;
using SharpDX;

namespace ReCrafted.Graphics
{
    /// <summary>
    /// Renderer class.
    /// </summary>
    public abstract class Renderer : IDisposable, ITickable
    {
        protected readonly Scene Scene;
        protected Rendering Rendering;

        /// <summary>
        /// Renderer constructor.
        /// </summary>
        protected Renderer()
        {
            Scene = new Scene();
        }

        /// <summary>
        /// Initializes the renderer.
        /// </summary>
        protected abstract void Init();

        /// <summary>
        /// Ticks the renderer.
        /// </summary>
        public abstract void Tick();

        /// <summary>
        /// Draws new frame.
        /// </summary>
        public abstract void Draw();

        /// <summary>
        /// Reizes the targets.
        /// </summary>
        /// <param name="width">Width</param>
        /// <param name="height">Height</param>
        public abstract void Resize(int width, int height);

        /// <summary>
        /// Sets rasterizer state.
        /// </summary>
        /// <param name="state">The state.</param>
        public abstract void SetRasterizerState(RasterizerStates state);

        /// <summary>
        /// Set the viewport size.
        /// </summary>
        /// <param name="width">The width.</param>
        /// <param name="height">The height.</param>
        public abstract void SetViewportSize(int width, int height);

        /// <summary>
        /// Sets the depth test state.
        /// </summary>
        /// <param name="enabled">Disable or enable the depth test?</param>
        public abstract void SetDepthTestState(bool enabled);

        /// <summary>
        /// Set RenderTargets as the current frame output.
        /// </summary>
        /// <param name="renderTargets">The RenderTargets.</param>
        public abstract void SetRenderTargets(params RenderTarget[] renderTargets);

        /// <summary>
        /// Set RenderTargets as the current frame output.
        /// </summary>
        /// <param name="depthRenderTarget">The depth render target.</param>
        /// <param name="renderTargets">The RenderTargets.</param>
        public abstract void SetRenderTargetsDepth(RenderTarget depthRenderTarget, params RenderTarget[] renderTargets);

        /// <summary>
        /// Set the final render target.
        /// </summary>
        /// <param name="useDepthTest">Use DepthTest?</param>
        public abstract void SetFinalRenderTarget(bool useDepthTest);

        /// <summary>
        /// Render/Copy the given RenderTarget to the current set RenderTarget.
        /// </summary>
        /// <param name="renderTarget">The render target.</param>
        /// <param name="customShader">The custom shader for blit. Must use that Vertex layout: { uint vert : SV_VERTEXID }</param>
        public abstract void Blit(RenderTarget renderTarget, Shader customShader = null);
        
        /// <summary>
        /// Clears depth.
        /// </summary>
        public abstract void ClearDepth();

        /// <summary>
        /// Dispatch ComputeShader.
        /// </summary>
        /// <param name="x">The X amount of threads.</param>
        /// <param name="y">The Y amount of threads.</param>
        /// <param name="z">The Z amount of threads.</param>
        public abstract void Dispatch(int x, int y, int z);

        /// <summary>
        /// Gets all RenderTargets from the current frame output.
        /// </summary>
        /// <returns>The RenderTargets.</returns>
        public abstract RenderTarget[] GetRenderTargets();

        /// <summary>
        /// Draw bounding box.
        /// </summary>
        /// <param name="bounds">The bouding box.</param>
        public abstract void DrawBoundingBox(BoundingBox bounds);

        /// <summary>
        /// Dispose the renderer.
        /// </summary>
        public abstract void Dispose();

        /// <summary>
        /// Creates renderer.
        /// </summary>
        /// <param name="rendererApi">The renderer api.</param>
        /// <param name="rendering">The rendering class instance.</param>
        public static void CreateRenderer(RendererApi rendererApi, Rendering rendering)
        {
            RendererApi = rendererApi;
            
            switch (rendererApi)
            {
                case RendererApi.D3D11:
                    Instance = new D3D11Renderer();
                    Instance.Init();

                    Instance.Rendering = rendering;
                    rendering.Init();
                    return;
            }

            // TODO: Implement renderers

            throw new ReCraftedException("Not implemented renderer!");
        }

        /// <summary>
        /// The depth render target.
        /// </summary>
        public RenderTarget DepthRenderTarget { get; protected set; }

        /// <summary>
        /// The renderer instance.
        /// </summary>
        public static Renderer Instance { get; protected set; }

        /// <summary>
        /// The current renderer api.
        /// </summary>
        public static RendererApi RendererApi { get; private set; }
    }
}