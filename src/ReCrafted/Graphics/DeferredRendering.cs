// ReCrafted © 2016 Damian 'Erdroy' Korczowski

using System.Collections.Generic;
using System.Linq;
using ReCrafted.Core;
using SharpDX;

namespace ReCrafted.Graphics
{
    /// <summary>
    /// DeferredRendering class.
    /// </summary>
    public sealed class DeferredRendering : Rendering
    {
        private List<RenderJob> _renderJobs = new List<RenderJob>();
        
        private RenderTarget _rtAlbedo;
        private RenderTarget _rtNormals;

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
            Game.Instance.OnResize += OnResize;

            // initialize all resources etc.
            _rtAlbedo = RenderTarget.Create(Display.ClientWidth, Display.ClientHeight);
            _rtNormals = RenderTarget.Create(Display.ClientWidth, Display.ClientHeight);
        }

        /// <summary>
        /// Draw everything, do new frame.
        /// </summary>
        public override void Draw()
        {
            _rtAlbedo.Clear(Camera.Current.BackgroundColor);
            _rtNormals.Clear(Color.Black);
            Renderer.Instance.SetRenderTargets(_rtAlbedo, _rtNormals);

            // do render jobs
            foreach (var job in _renderJobs)
            {
                job.JobMethod(this);
            }

            Renderer.Instance.SetFinalRenderTarget(false);
            Renderer.Instance.Blit(_rtAlbedo);
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
            _rtAlbedo?.Dispose();
            _rtNormals?.Dispose();
        }

        // private
        private void OnResize(int width, int height)
        {
            // resize render targets
            _rtAlbedo.Resize(width, height);
            _rtNormals.Resize(width, height);
        }
    }
}