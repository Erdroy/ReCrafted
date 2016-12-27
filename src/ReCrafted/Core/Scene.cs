// ReCrafted © 2016 Damian 'Erdroy' Korczowski

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
        private Sampler _pointSampler;

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

            _simplePost = Shader.FromFile("postprocess/vingetting");
            _pointSampler = Sampler.Create(Sampler.Type.PointClamped);

            _simplePost.SetSampler(0, _pointSampler);

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
    }
}