// ReCrafted © 2016 Damian 'Erdroy' Korczowski

using System;

namespace ReCrafted.Graphics
{
    /// <summary>
    /// Rendering class.
    /// </summary>
    public abstract class Rendering : IDisposable
    {
        /// <summary>
        /// Initialize rendering pipeline.
        /// </summary>
        public abstract void Init();
        
        /// <summary>
        /// Draw everything, do new frame.
        /// </summary>
        public abstract void Draw();

        /// <summary>
        /// Add render job to the pipeline.
        /// </summary>
        /// <param name="job">The render job.</param>
        public abstract void AddRenderJob(RenderJob job);
        
        /// <summary>
        /// Add post deferred render job.
        /// </summary>
        /// <param name="job">The render job.</param>
        public abstract void AddPostDeferredRenderJob(RenderJob job);

        /// <summary>
        /// Dispose the rendering
        /// </summary>
        public abstract void Dispose();
        
        /// <summary>
        /// The current rendering.
        /// </summary>
        public static Rendering Current { get; set; }
    }
}