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
        /// Set the final render target.
        /// </summary>
        /// <param name="useDepthTest">Use DepthTest?</param>
        public abstract void SetFinalRenderTarget(bool useDepthTest);

        /// <summary>
        /// Render/Copy the given RenderTarget to the current set RenderTarget.
        /// </summary>
        /// <param name="renderTarget">The render target.</param>
        public abstract void Blit(RenderTarget renderTarget);

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
            
            if (rendererApi == RendererApi.D3D11)
            {
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
        /// The renderer instance.
        /// </summary>
        public static Renderer Instance { get; protected set; }

        /// <summary>
        /// The current renderer api.
        /// </summary>
        public static RendererApi RendererApi { get; private set; }
    }
}