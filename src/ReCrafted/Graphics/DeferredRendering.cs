// ReCrafted © 2016 Damian 'Erdroy' Korczowski

using System.Collections.Generic;
using System.Linq;

namespace ReCrafted.Graphics
{
    /// <summary>
    /// DeferredRendering class.
    /// </summary>
    public sealed class DeferredRendering : Rendering
    {
        private List<RenderJob> _renderJobs = new List<RenderJob>();

        private Shader _renderGBuffer;

        /// <summary>
        /// Default constructor of DeferredRendering class.
        /// </summary>
        public DeferredRendering()
        {
            // set the current rendering instance reference to
            // the current instace. yey.
            Current = this;
        }

        /// <summary>
        /// Initialize rendering pipeline.
        /// </summary>
        public override void Init()
        {
            // initialize all resources etc.

            // load shaders
            // the GBuffer render shader
            _renderGBuffer = Shader.FromFile("Render_GBuffer");
        }

        /// <summary>
        /// Draw everything, do new frame.
        /// </summary>
        public override void Draw()
        {
            // do render jobs
            foreach (var job in _renderJobs)
            {
                job.JobMethod(this);
            }
        }

        /// <summary>
        /// Add render job to the pipeline.
        /// </summary>
        /// <param name="job">The render job.</param>
        public override void AddRenderJob(RenderJob job)
        {
            // add new render job
            _renderJobs.Add(job);

            // sort all of the render jobs
            // using render priority
            _renderJobs = _renderJobs.OrderBy(renderJob => renderJob.RenderPriority).ToList();
        }
        
        /// <summary>
        /// Dispose the rendering
        /// </summary>
        public override void Dispose()
        {
            // clear render jobs,
            _renderJobs.Clear();

            // release all resources
            _renderGBuffer?.Dispose();
        }
    }
}