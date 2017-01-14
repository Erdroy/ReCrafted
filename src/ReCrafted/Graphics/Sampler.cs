// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

using System;
using ReCrafted.Graphics.Renderers.D3D11;
using ReCrafted.Graphics.Renderers.OpenGL;
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
            switch (Renderer.RendererApi)
            {
                case RendererApi.D3D11:
                    {
                        var sampler = new D3D11Sampler();
                        sampler.Init(type, anisoLevel);
                        return sampler;
                    }
                case RendererApi.OpenGL:
                    {
                        var sampler = new OpenGLSampler();
                        sampler.Init(type, anisoLevel);
                        return sampler;
                    }
            }

            // TODO: Implement renderers

            throw new ReCraftedException("Invalid RendererApi!");
        }

        /// <summary>
        /// The type of the sampler.
        /// </summary>
        public Type SamplerType { get; protected set; }
    }
}