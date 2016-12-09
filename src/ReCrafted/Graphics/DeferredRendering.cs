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
        private List<RenderJob> _postRenderJobs = new List<RenderJob>();

        private RenderTarget _rtAlbedo;
        private RenderTarget _rtNormals;

        private RenderTarget _rtFinal;

        private Shader _finalShader;

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

            _rtFinal = RenderTarget.Create(Display.ClientWidth, Display.ClientHeight, true);

            _finalShader = Shader.FromFile("Render_Final");
        }

        /// <summary>
        /// Draw everything, do new frame.
        /// </summary>
        public override void Draw()
        {
            _rtAlbedo.Clear(Camera.Current.BackgroundColor);
            _rtNormals.Clear(Color.Black);
            _rtFinal.Clear(Color.Gray);

            Renderer.Instance.SetRenderTargets(_rtAlbedo, _rtNormals);

            // do render jobs
            foreach (var job in _renderJobs)
            {
                job.JobMethod(this);
            }

            // do final pass
            ComputeOutput();

            // present to the swapchain's FinalRT
            Renderer.Instance.SetFinalRenderTarget(false);
            Renderer.Instance.Blit(_rtFinal);

            // do render jobs
            Renderer.Instance.SetFinalRenderTarget(true);
            foreach (var job in _postRenderJobs)
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
        /// Add post deferred render job.
        /// </summary>
        /// <param name="job">The render job.</param>
        public override void AddPostDeferredRenderJob(RenderJob job)
        {
            // add new render job

            _postRenderJobs.Add(job);

            // sort all of the render jobs
            // using render priority
            _postRenderJobs = _postRenderJobs.OrderBy(renderJob => renderJob.RenderPriority).ToList();
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
            _rtFinal?.Dispose();
        }

        // private
        private void OnResize(int width, int height)
        {
            // resize render targets
            _rtAlbedo.Resize(width, height);
            _rtNormals.Resize(width, height);
            _rtFinal.Resize(width, height);
        }

        // private
        private void ComputeOutput()
        {
            // Temporary fix(the Albedo and Normals RT are bound to the output, 
            // override the output - allof' should be null)
            Renderer.Instance.SetFinalRenderTarget(false);

            _finalShader.Apply();

            // set resources
            _finalShader.SetUnorderedAccessView(0, _rtFinal);
            _finalShader.SetRenderTexture(ShaderType.CS, 0, _rtAlbedo);
            _finalShader.SetRenderTexture(ShaderType.CS, 1, _rtNormals);

            // dispatch
            var xThreads = DispatchSize(16, Display.ClientWidth);
            var yThreads = DispatchSize(16, Display.ClientHeight);
            Renderer.Instance.Dispatch(xThreads, yThreads, 1);

            // unset resources
            _finalShader.UnsetUnorderedAccessView(0);
            _finalShader.UnsetRenderTexture(ShaderType.CS, 0);
            _finalShader.UnsetRenderTexture(ShaderType.CS, 1);

        }

        // private
        private static int DispatchSize(int tgSize, int numElements)
        {
            return numElements / tgSize + (numElements % tgSize > 0 ? 1 : 0);
        }
    }
}