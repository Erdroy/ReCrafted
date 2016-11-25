// ReCrafted © 2016 Damian 'Erdroy' Korczowski

using System;
using ReCrafted.Core;
using ReCrafted.Graphics.Renderers.D3D11;
using ReCrafted.Logic;
using ReCrafted.Utilities;

namespace ReCrafted.Graphics
{
    /// <summary>
    /// Renderer class.
    /// </summary>
    public abstract class Renderer : IDisposable, ITickable
    {
        /// <summary>
        /// The renderer instance.
        /// </summary>
        public static Renderer Instance;

        /// <summary>
        /// The current renderer api.
        /// </summary>
        public static RendererApi RendererApi;

        private readonly Scene _scene;

        /// <summary>
        /// Renderer constructor.
        /// </summary>
        protected Renderer()
        {
            _scene = new Scene();
        }

        /// <summary>
        /// Initializes the renderer.
        /// </summary>
        protected virtual void Init()
        {
            _scene.Init();
        }

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
        /// Dispose the renderer.
        /// </summary>
        public abstract void Dispose();

        /// <summary>
        /// Creates renderer.
        /// </summary>
        /// <param name="rendererApi">The renderer api.</param>
        public static void CreateRenderer(RendererApi rendererApi)
        {
            RendererApi = rendererApi;

            if (rendererApi == RendererApi.D3D11)
            {
                Instance = new D3D11Renderer();
                Instance.Init();
                return;
            }

            // TODO: Implement renderers

            throw new ReCraftedException("Not implemented renderer!");
        }
    }
}