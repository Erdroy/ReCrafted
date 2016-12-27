// ReCrafted © 2016 Damian 'Erdroy' Korczowski

namespace ReCrafted.Graphics
{
    /// <summary>
    /// Render job method delegate.
    /// </summary>
    /// <param name="rendering">The current Rendering class instance.</param>
    public delegate void RenderJobMethod(Rendering rendering);

    /// <summary>
    /// RenderJob class.
    /// </summary>
    public struct RenderJob
    {
        /// <summary>
        /// The JobMethod.
        /// </summary>
        public RenderJobMethod JobMethod { get; set; }

        /// <summary>
        /// The RenderPriority.
        /// </summary>
        public int RenderPriority { get; set; }
    }
}