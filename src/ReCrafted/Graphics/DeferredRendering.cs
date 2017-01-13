// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

using System.Collections.Generic;
using System.Linq;
using ReCrafted.Core;
using ReCrafted.Graphics.Primitives;
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

        private List<PostprocessJob> _postprocessJobs = new List<PostprocessJob>();

        private RenderTarget _rtAlbedo;
        private RenderTarget _rtNormals;
        private RenderTarget _rtAmbientOcculusion;
        private RenderTarget _rtFinal;
        private RenderTarget _rtOutput;

        private Shader _finalShader;

        private Sampler _pointSampler;

        // skybox
        private Shader _skyboxShader;
        private Mesh _skyboxSphere;

        private ShadowRenderer _shadowRenderer;

        private Vector3 _ligthDirection;

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
            _rtAlbedo = RenderTarget.Create(Display.ClientWidth, Display.ClientHeight, RenderTarget.TextureFormat.RGBA8_UNorm);
            _rtNormals = RenderTarget.Create(Display.ClientWidth, Display.ClientHeight, RenderTarget.TextureFormat.RGBA8_UNorm);
            _rtAmbientOcculusion = RenderTarget.Create(Display.ClientWidth, Display.ClientHeight, RenderTarget.TextureFormat.R16_Float);
            _rtFinal = RenderTarget.Create(Display.ClientWidth, Display.ClientHeight, RenderTarget.TextureFormat.RGBA8_UNorm);
            _rtOutput = RenderTarget.Create(Display.ClientWidth, Display.ClientHeight, RenderTarget.TextureFormat.RGBA8_UNorm);

            _skyboxSphere = Mesh.FromMeshData(new IcoSphere(3, 1.0f, true).GetMeshData());

            _pointSampler = Sampler.Create(Sampler.Type.PointClamped);

            _finalShader = Shader.FromFile("render_main");
            _skyboxShader = Shader.FromFile("Skybox");
            
            _shadowRenderer = new ShadowRenderer();
            _shadowRenderer.Init();
            _ligthDirection = -new Vector3(0.4f, 0.4f, 0.2f);
        }

        /// <summary>
        /// Draw everything, do new frame.
        /// </summary>
        public override void Draw()
        {
            _rtAlbedo.Clear(Camera.Current.BackgroundColor);
            _rtNormals.Clear(Color.Black);
            _rtAmbientOcculusion.Clear(Color.Black);
            _rtFinal.Clear(Color.Black);
            _rtOutput.Clear(Color.Black);

            // render skybox into final RT

            Renderer.Instance.FaceCulling(true, false);
            Renderer.Instance.SetRenderTargets(_rtAlbedo);
            _skyboxShader.Apply();
            _skyboxShader.SetValue("WVP", Matrix.Translation(Camera.Current.Position) * Camera.Current.ViewProjectionMatrix);
            _skyboxShader.SetValue("ColorUpper", new Vector4(0.0f, 0.3f, 0.4f, 1.0f));
            _skyboxShader.SetValue("ColorMiddle", new Vector4(0.0f, 0.6f, 0.8f, 1.0f));
            _skyboxShader.SetValue("ColorLower", new Vector4(0.0f, 0.1f, 0.1f, 1.0f));
            _skyboxShader.ApplyChanges();
            _skyboxShader.Draw(_skyboxSphere);
            Renderer.Instance.FaceCulling(false, true);

            // clear depth
            Renderer.Instance.ClearDepth();

            // set gbuffer render targets
            Renderer.Instance.SetRenderTargets(_rtAlbedo, _rtNormals, _rtAmbientOcculusion);
            Renderer.Instance.SetDepthTestState(true);

            // do render jobs
            foreach (var job in _renderJobs)
            {
                job.JobMethod(this);
            }


#if OPENGL
            Renderer.Instance.SetFinalRenderTarget(false);
            Renderer.Instance.Blit(_rtAlbedo);
#endif

#if D3D11
            // render shadows
            _shadowRenderer.LightDir = _ligthDirection;
            _shadowRenderer.RenderShadowMap();

            // do final pass
            RenderFinal();

            // do post process
            var input = _rtFinal;
            var output = _rtOutput;

            foreach (var job in _postprocessJobs)
            {
                Renderer.Instance.SetRenderTargets(output);
                Renderer.Instance.SetDepthTestState(false);

                job.JobMethod(this, input, output);

                var tmp = output;
                output = input;
                input = tmp;
            }

            // present to the swapchain's FinalRT
            Renderer.Instance.SetFinalRenderTarget(false);
            Renderer.Instance.Blit(input);

            // do render jobs
            Renderer.Instance.SetFinalRenderTarget(true);
            foreach (var job in _postRenderJobs)
            {
                job.JobMethod(this);
            }
#endif
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
        /// Add postprocess job.
        /// </summary>
        /// <param name="job">The postprocess job.</param>
        public override void AddPostprocessJob(PostprocessJob job)
        {
            _postprocessJobs.Add(job);
            
            // render priority
            _postprocessJobs = _postprocessJobs.OrderBy(renderJob => renderJob.RenderPriority).ToList();
        }

        /// <summary>
        /// Dispose the rendering
        /// </summary>
        public override void Dispose()
        {
            // clear render jobs,
            _renderJobs.Clear();
            _postRenderJobs.Clear();
            _postprocessJobs.Clear();

            // release all resources
            _rtAlbedo?.Dispose();
            _rtNormals?.Dispose();
            _rtAmbientOcculusion?.Dispose();
            _rtFinal?.Dispose();
            _rtOutput?.Dispose();

            _skyboxSphere?.Dispose();
            
            _skyboxShader?.Dispose();
            _finalShader?.Dispose();
            _shadowRenderer?.Dispose();
        }

        // private
        private void OnResize(int width, int height)
        {
            // resize render targets
            _rtAlbedo.Resize(width, height);
            _rtNormals.Resize(width, height);
            _rtAmbientOcculusion.Resize(width, height);
            _rtFinal.Resize(width, height);
            _rtOutput.Resize(width, height);

            _shadowRenderer.Resize(width, height);
        }

        // private
        private void RenderFinal()
        {
            Renderer.Instance.SetRenderTargets(_rtFinal);
            Renderer.Instance.SetDepthTestState(false);

            _finalShader.Apply();

            _finalShader.SetValue("LightColor", new Vector4(1.0f, 1.0f, 1.0f, 1.6f));
            _finalShader.SetValue("LightDir", _ligthDirection);
            
            _finalShader.SetRenderTexture(ShaderType.PS, 0, _rtAlbedo);
            _finalShader.SetRenderTexture(ShaderType.PS, 1, _rtNormals);
            _finalShader.SetRenderTexture(ShaderType.PS, 2, _rtAmbientOcculusion);
            _finalShader.SetRenderTexture(ShaderType.PS, 3, Renderer.Instance.DepthRenderTarget);
            _finalShader.SetRenderTexture(ShaderType.PS, 4, _shadowRenderer.ShadowOcculusion);

            _finalShader.SetSampler(0, _pointSampler);

            _finalShader.ApplyChanges();

            _finalShader.DrawFullscreen();

            _finalShader.UnsetRenderTexture(ShaderType.PS, 0);
            _finalShader.UnsetRenderTexture(ShaderType.PS, 1);
            _finalShader.UnsetRenderTexture(ShaderType.PS, 2);
            _finalShader.UnsetRenderTexture(ShaderType.PS, 3);
            _finalShader.UnsetRenderTexture(ShaderType.PS, 4);
        }
    }
}