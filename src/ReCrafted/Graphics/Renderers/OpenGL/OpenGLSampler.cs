// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

namespace ReCrafted.Graphics.Renderers.OpenGL
{
    /// <summary>
    /// OpenGLSampler class.
    /// </summary>
    // ReSharper disable once InconsistentNaming
    internal sealed class OpenGLSampler : Sampler
    {
        /// <summary>
        /// Initialize sampler.
        /// </summary>
        /// <param name="type">The sampler type.</param>
        /// <param name="anisoLevel">The anisotropic level.</param>
        protected override void Init(Type type, int anisoLevel)
        {
        }

        /// <summary>
        /// Apply the sampler.
        /// </summary>
        /// <param name="slot">The slot.</param>
        public override void Apply(int slot)
        {
        }

        /// <summary>
        /// Dispose the sampler.
        /// </summary>
        public override void Dispose()
        {
        }
    }
}