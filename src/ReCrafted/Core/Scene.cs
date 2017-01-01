// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

using System;
using ReCrafted.Graphics;
using ReCrafted.Voxels;
using SharpDX;

namespace ReCrafted.Core
{
    /// <summary>
    /// Scene class.
    /// </summary>
    public sealed class Scene : IDisposable
    {
        /// <summary>
        /// Scene instance.
        /// </summary>
        public static Scene Instance;

        private VoxelWorld _voxelWorld;
        private VoxelCursor _voxelCursor;
        private Camera _camera;

        private Shader _simplePost;
        private Shader _fxaaPost;
        private Sampler _pointSampler;
        private Sampler _linearSampler;

        /// <summary>
        /// Scene constructor.
        /// </summary>
        internal Scene()
        {
            Instance = this;
        }

        /// <summary>
        /// Initializes the scene.
        /// Called by Rendering class.
        /// </summary>
        public void Init()
        {
            _voxelWorld = new VoxelWorld();
            _voxelWorld.Init();

            _voxelCursor = new VoxelCursor();
            _voxelCursor.Init();

            _pointSampler = Sampler.Create(Sampler.Type.PointClamped);
            _linearSampler = Sampler.Create(Sampler.Type.LinearClamped);
            _simplePost = Shader.FromFile("postprocess/vingetting");
            _fxaaPost = Shader.FromFile("postprocess/fxaa");

            _simplePost.SetSampler(0, _pointSampler);
            _fxaaPost.SetSampler(0, _linearSampler);

            // initialize camera
            _camera = new Camera
            {
                BackgroundColor = new Color(25, 25, 25, 255),
                Position = new Vector3(0.0f, 60.0f, 10.0f),
                FreeLook = true,
                FieldOfView = (float)Math.PI * 0.4f
            };

            _camera.SetAsCurrent();

            // Add render jobs
            Rendering.Current.AddRenderJob(new RenderJob
            {
                JobMethod = RenderScene,
                RenderPriority = 0
            });

            // Add post render jobs
            Rendering.Current.AddPostDeferredRenderJob(new RenderJob
            {
                JobMethod = RenderCursor,
                RenderPriority = 0
            });

            Rendering.Current.AddPostprocessJob(new PostprocessJob
            {
                RenderPriority = 0,
                JobMethod = PostSimple
            });
            /*Rendering.Current.AddPostprocessJob(new PostprocessJob
            {
                RenderPriority = 0,
                JobMethod = PostFxaa
            });*/
        }
        
        /// <summary>
        /// Ticks the scene, entity pool etc.
        /// Called by Game class.
        /// </summary>
        public void Tick()
        {
            Camera.Current.Tick();
            Renderer.Instance.Tick();

            // tick the voxel cursor
            _voxelCursor.Tick();

            _voxelWorld.Tick();
            EntityPool.Tick();
        }

        /// <summary>
        /// Simulates the scene, entity pool etc.
        /// Called by Game class.
        /// </summary>
        public void Simulate()
        {
            _voxelWorld.Simulate();
            EntityPool.Simulate();
        }

        /// <summary>
        /// Dispose the scene.
        /// </summary>
        public void Dispose()
        {
            _voxelWorld.Dispose();

            _pointSampler?.Dispose();
            _simplePost?.Dispose();
            _fxaaPost?.Dispose();
        }

        // private
        private void RenderScene(Rendering rendering)
        {
            _voxelWorld.Draw();
            EntityPool.Draw();

        }

        // private
        private void RenderCursor(Rendering rendering)
        {
            _voxelCursor.Draw();
        }

        // private
        private void PostSimple(Rendering rendering, RenderTarget input, RenderTarget output)
        {
            _simplePost.Apply();

            Renderer.Instance.Blit(input, _simplePost);
        }

        // private
        private void PostFxaa(Rendering rendering, RenderTarget input, RenderTarget output)
        {
            /*_fxaaPost.Apply();
            _fxaaPost.SetValue("Viewport", new Vector4(1.0f / Display.ClientWidth, 1.0f / Display.ClientHeight, 0.0f, 0.0f));
            _fxaaPost.ApplyChanges();
            
            Renderer.Instance.Blit(input, _fxaaPost);*/
        }
    }
}