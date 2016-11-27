// ReCrafted © 2016 Damian 'Erdroy' Korczowski

using System;
using ReCrafted.Graphics;
using ReCrafted.Utilities;
using ReCrafted.Voxels;
using SharpDX;

namespace ReCrafted.Core
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
        private VoxelCursor _voxelCursor;
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

            _voxelCursor = new VoxelCursor();
            _voxelCursor.Init();

            // initialize camera
            _camera = new Camera
            {
                BackgroundColor = new Color(25, 25, 25, 255),
                Position = new Vector3(0.0f, 2.0f, -10.0f),
                FreeLook = true,
                FieldOfView = 0.85f
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

            _voxelCursor.Draw();
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