// ReCrafted © 2016 Damian 'Erdroy' Korczowski

using System;
using System.Collections.Generic;
using ReCrafted.Core;

namespace ReCrafted.Voxels
{
    /// <summary>
    /// VoxelWorld class.
    /// </summary>
    public sealed class VoxelWorld : IDisposable, IRendererComponent
    {
        /// <summary>
        /// The VoxelWorld instance.
        /// </summary>
        public static VoxelWorld Instance;
        
        private readonly List<VoxelChunk> _chunks = new List<VoxelChunk>();

        /// <summary>
        /// VoxelWorld constructor.
        /// </summary>
        public VoxelWorld()
        {
            Instance = this;
        }

        /// <summary>
        /// Initializes the VoxelWorld.
        /// </summary>
        public void Init()
        {
            VoxelAssets.LoadAssets();

            var chunk = new VoxelChunk();
            chunk.Init();
            chunk.UpdateMesh();

            _chunks.Add(chunk);
        }

        /// <summary>
        /// Ticks the VoxelWorld.
        /// </summary>
        public void Tick()
        {
            foreach (var chunk in _chunks)
                chunk.Tick();
        }

        /// <summary>
        /// Simulates the VoxelWorld.
        /// </summary>
        public void Simulate()
        {
        }

        /// <summary>
        /// Draws the VoxelWorld.
        /// </summary>
        public void Draw()
        {
            foreach (var chunk in _chunks)
                chunk.Draw();
        }

        /// <summary>
        /// Disposes the VoxelWorld.
        /// </summary>
        public void Dispose()
        {
            foreach (var chunk in _chunks)
                chunk.Dispose();
        }
    }
}