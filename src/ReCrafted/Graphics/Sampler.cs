// ReCrafted © 2016 Damian 'Erdroy' Korczowski

using System;
using ReCrafted.Graphics.Renderers.D3D11;
using ReCrafted.Utilities;

namespace ReCrafted.Graphics
{
    /// <summary>
    /// Sampler class.
    /// </summary>
    public abstract class Sampler : IDisposable
    {
        /// <summary>
        /// Sampler types
        /// </summary>
        public enum Type
        {
            PointClamped,
            PointWrap,
            PointMirror,

            LinearClamped,
            LinearWrap,
            LinearMirror,

            AnisoClamped,
            AnisoWrap,
            AnisoMirror,
        }
        
        // lock from creating
        protected Sampler() { }
        
        /// <summary>
        /// Initialize sampler.
        /// </summary>
        /// <param name="type">The sampler type.</param>
        /// <param name="anisoLevel">The anisotropic level.</param>
        protected abstract void Init(Type type, int anisoLevel);

        /// <summary>
        /// Apply the sampler.
        /// </summary>
        /// <param name="slot">The slot.</param>
        public abstract void Apply(int slot);

        /// <summary>
        /// Dispose the sampler.
        /// </summary>
        public abstract void Dispose();
        
        /// <summary>
        /// Creates new sampler.
        /// </summary>
        /// <param name="type">The sampler type.</param>
        /// <param name="anisoLevel">The anisotropic level. Not used when non-aniso.</param>
        /// <returns>The created sampler.</returns>
        public static Sampler Create(Type type, int anisoLevel = 16)
        {
            if (Renderer.RendererApi == RendererApi.D3D11)
            {
                var d3D11Sampler = new D3D11Sampler();
                d3D11Sampler.Init(type, anisoLevel);
                return d3D11Sampler;
            }

            // TODO: Implement renderers

            throw new ReCraftedException("Invalid RendererApi!");
        }
    }
}