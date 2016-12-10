// ReCrafted © 2016 Damian 'Erdroy' Korczowski

using System.Collections.Generic;
using System.Linq;
using ReCrafted.Core;
using ReCrafted.Graphics.Primitives;
using ReCrafted.Voxels;
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
        private RenderTarget _rtShadowMap;
        private RenderTarget _rtFinal;

        private Shader _finalShader;

        // skybox
        private Shader _skyboxShader;
        private Mesh _skyboxSphere;

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
            _rtAlbedo = RenderTarget.Create(Display.ClientWidth, Display.ClientHeight);
            _rtNormals = RenderTarget.Create(Display.ClientWidth, Display.ClientHeight);
            _rtShadowMap = RenderTarget.Create(Display.ClientWidth, Display.ClientHeight);

            _rtFinal = RenderTarget.Create(Display.ClientWidth, Display.ClientHeight, true);

            _skyboxSphere = Mesh.FromMeshData(new IcoSphere(3, 1.0f, true).GetMeshData());

            _finalShader = Shader.FromFile("Render_Final");
            _skyboxShader = Shader.FromFile("Skybox");


            _ligthDirection = -new Vector3(0.4f, 0.4f, 0.2f);
        }

        /// <summary>
        /// Draw everything, do new frame.
        /// </summary>
        public override void Draw()
        {
            _rtAlbedo.Clear(Camera.Current.BackgroundColor);
            _rtNormals.Clear(Color.Black);
            _rtFinal.Clear(Color.Black);
            _rtShadowMap.Clear(Color.Black);

            // render skybox into final RT
            Renderer.Instance.SetRenderTargets(_rtAlbedo);
            Renderer.Instance.SetDepthTestState(false);
            _skyboxShader.Apply();
            _skyboxShader.SetValue("WVP", Matrix.Translation(Camera.Current.Position) * Camera.Current.ViewProjectionMatrix);
            _skyboxShader.SetValue("ColorUpper", new Vector4(0.0f, 0.3f, 0.4f, 1.0f));
            _skyboxShader.SetValue("ColorMiddle", new Vector4(0.0f, 0.6f, 0.8f, 1.0f));
            _skyboxShader.SetValue("ColorLower", new Vector4(0.0f, 0.1f, 0.1f, 1.0f));
            _skyboxShader.ApplyChanges();
            _skyboxShader.Draw(_skyboxSphere);
            Renderer.Instance.SetDepthTestState(true);

            // render shadow map
            GenerateShadowMap();

            // clear depth
            Renderer.Instance.ClearDepth();

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
            _postRenderJobs.Clear();

            // release all resources
            _rtAlbedo?.Dispose();
            _rtNormals?.Dispose();
            _rtFinal?.Dispose();
            _rtShadowMap?.Dispose();

            _skyboxSphere?.Dispose();

            _skyboxShader?.Dispose();
            _finalShader?.Dispose();
        }

        // private
        private void OnResize(int width, int height)
        {
            // resize render targets
            _rtAlbedo.Resize(width, height);
            _rtNormals.Resize(width, height);
            _rtFinal.Resize(width, height);
            _rtShadowMap.Resize(width, height);
        }

        // private
        private void GenerateShadowMap()
        {
            Renderer.Instance.SetRenderTargets(_rtShadowMap);

            var lookAt = Matrix.LookAtLH(Camera.Current.Position, Camera.Current.Position - _ligthDirection, Vector3.Up);
            var proj = Matrix.OrthoOffCenterLH(-25, 25, -25, 25, -100, 200);
            
            VoxelWorld.Instance.DrawShadowMap(lookAt * proj);
        }

        // private
        private void ComputeOutput()
        {
            Renderer.Instance.SetFinalRenderTarget(false);

            _finalShader.Apply();

            _finalShader.SetValue("LightColor", new Color(1.0f, 1.0f, 1.0f, 1.0f).ToVector4());
            _finalShader.SetValue("LightDir", _ligthDirection);

            // set resources
            _finalShader.SetUnorderedAccessView(0, _rtFinal);
            _finalShader.SetRenderTexture(ShaderType.CS, 0, _rtAlbedo);
            _finalShader.SetRenderTexture(ShaderType.CS, 1, _rtNormals);
            _finalShader.SetRenderTexture(ShaderType.CS, 2, _rtShadowMap);

            // dispatch
            var xThreads = DispatchSize(16, Display.ClientWidth);
            var yThreads = DispatchSize(16, Display.ClientHeight);

            _finalShader.ApplyChanges();
            Renderer.Instance.Dispatch(xThreads, yThreads, 1);

            // unset resources
            _finalShader.UnsetUnorderedAccessView(0);
            _finalShader.UnsetRenderTexture(ShaderType.CS, 0);
            _finalShader.UnsetRenderTexture(ShaderType.CS, 1);
            _finalShader.UnsetRenderTexture(ShaderType.CS, 2);

        }

        // private
        private static int DispatchSize(int tgSize, int numElements)
        {
            return numElements / tgSize + (numElements % tgSize > 0 ? 1 : 0);
        }
    }
}