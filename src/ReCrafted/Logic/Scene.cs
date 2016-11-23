using System;
using ReCrafted.Core;
using ReCrafted.Graphics;
using ReCrafted.Voxels;
using SharpDX;

namespace ReCrafted.Logic
{
    /// <summary>
    /// Scene class.
    /// </summary>
    public sealed class Scene : IDisposable, IRendererComponent
    {
        /// <summary>
        /// Scene instance.
        /// </summary>
        public static Scene Instance;

        private VoxelWorld _voxelWorld;
        private Camera _camera;

        /// <summary>
        /// Scene constructor.
        /// </summary>
        internal Scene()
        {
            Instance = this;
        }

        /// <summary>
        /// Initializes the scene.
        /// </summary>
        public void Init()
        {
            _voxelWorld = new VoxelWorld();
            _voxelWorld.Init();

            // initialize camera
            _camera = new Camera
            {
                BackgroundColor = Color.CornflowerBlue,
                Position = new Vector3(0.0f, 2.0f, -10.0f),
                FreeLook = true
            };

            _camera.SetAsCurrent();
        }

        /// <summary>
        /// Ticks the scene, entity pool etc.
        /// </summary>
        public void Tick()
        {
            Camera.Current.Tick();
            Renderer.Instance.Tick();

            _voxelWorld.Tick();
            EntityPool.Tick();
        }
        
        /// <summary>
        /// Simulates the scene, entity pool etc.
        /// </summary>
        public void Simulate()
        {
            _voxelWorld.Simulate();
            EntityPool.Simulate();
        }
        
        /// <summary>
        /// Draws the scene, entity pool etc.
        /// </summary>
        public void Draw()
        {
            _voxelWorld.Draw();
            EntityPool.Draw();
        }

        /// <summary>
        /// Dispose the scene.
        /// </summary>
        public void Dispose()
        {
            _voxelWorld.Dispose();
        }
    }
}