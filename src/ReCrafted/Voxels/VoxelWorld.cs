// ReCrafted © 2016 Damian 'Erdroy' Korczowski

using System;
using System.Collections.Generic;
using ReCrafted.Core;
using ReCrafted.Graphics;
using SharpDX;

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

            if (Input.IsButtonDown(ButtonCode.Left))
            {
                var pos = VoxelCursor.Instance.BlockCoords;
                _chunks[0].SetBlock(0, pos.X, pos.Y, pos.Z);
            }

            if (Input.IsButtonDown(ButtonCode.Middle))
            {
                var position = Camera.Current.Position + Camera.Current.Forward * 7;
                var worldCoord = new Int3((int)position.X, (int)position.Y, (int)position.Z);

                if (worldCoord.X < 0)
                    worldCoord.X = 0;

                if (worldCoord.Y < 0)
                    worldCoord.Y = 0;

                if (worldCoord.Z < 0)
                    worldCoord.Z = 0;

                if (worldCoord.X > 15)
                    worldCoord.X = 15;

                if (worldCoord.Y > 15)
                    worldCoord.Y = 15;

                if (worldCoord.Z > 15)
                    worldCoord.Z = 15;

                _chunks[0].SetBlock(1, worldCoord.X, worldCoord.Y, worldCoord.Z);
            }
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
        /// Raycast blocks through the world to get block hit and chunk.
        /// </summary>
        /// <param name="origin">The origin.</param>
        /// <param name="direction">The normalized direction.</param>
        /// <param name="distance">The maximum distance for the ray.</param>
        /// <param name="blockId">The Block id.</param>
        /// <param name="coord">Block coord id.</param>
        /// <param name="chunk">The hit VoxelChunk, null when no any blocks were hit.</param>
        public void RaycastBlocks(Vector3 origin, Vector3 direction, int distance, out ushort blockId, out Int3 coord, out VoxelChunk chunk)
        {
            blockId = 0;
            coord = Int3.Zero;
            chunk = null;

            const float precision = 100.0f;
            
            // for default 7 blocks distance there is 700 iterations, TODO: needs some huge optimizations.
            for (var i = 0; i < distance * precision; i++)
            {
                var actualDistance = (i + 1) * (1 / precision);
                var position = origin + direction * actualDistance;

                var worldCoord = new Int3((int)position.X, (int)position.Y, (int)position.Z);

                // TODO: Multi chunk

                if (worldCoord.X < 0)
                    worldCoord.X = 0;

                if (worldCoord.Y < 0)
                    worldCoord.Y = 0;

                if (worldCoord.Z < 0)
                    worldCoord.Z = 0;

                if (worldCoord.X > 15)
                    worldCoord.X = 15;

                if (worldCoord.Y > 15)
                    worldCoord.Y = 15;

                if (worldCoord.Z > 15)
                    worldCoord.Z = 15;

                var block = _chunks[0].GetBlock(worldCoord.X, worldCoord.Y, worldCoord.Z);
                
                if (block != 0)
                {
                    chunk = _chunks[0];
                    blockId = block;
                    coord = worldCoord;
                    return;
                }
            }
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