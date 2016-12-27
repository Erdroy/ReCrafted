// ReCrafted © 2016 Damian 'Erdroy' Korczowski

namespace ReCrafted.Graphics
{
    /// <summary>
    /// Render job method delegate.
    /// </summary>
    /// <param name="rendering">The current Rendering class instance.</param>
    public delegate void PostprocessJobMethod(Rendering rendering, RenderTarget input, RenderTarget output);

    /// <summary>
    /// RenderJob class.
    /// </summary>
    public struct PostprocessJob
    {
        /// <summary>
        /// The JobMethod.
        /// </summary>
        public PostprocessJobMethod JobMethod { get; set; }

        /// <summary>
        /// The RenderPriority.
        /// </summary>
        public int RenderPriority { get; set; }
    }
}