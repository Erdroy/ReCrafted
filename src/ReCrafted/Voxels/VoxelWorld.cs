// ReCrafted © 2016 Damian 'Erdroy' Korczowski

using System;
using System.Collections.Generic;
using System.Linq;
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
        // const
        /// <summary>
        /// The chunk width/height.
        /// </summary>
        public const int ChunkSize = 16;

        /// <summary>
        /// The chunk height.
        /// </summary>
        public const int ChunkHeight = 255;

        /// <summary>
        /// The block size.
        /// </summary>
        public const float BlockSize = 1.0f;

        // fields
        /// <summary>
        /// The VoxelWorld instance.
        /// </summary>
        public static VoxelWorld Instance;
        
        private readonly List<VoxelChunk> _chunks = new List<VoxelChunk>();

        // methods
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

            // create chunks
            for (var z = 0; z < 1; z++)
            {
                for (var x = 0; x < 1; x++)
                {
                    var chunk = new VoxelChunk
                    {
                        Position = new Int3(x * 16, 0, z * 16)
                    };
                    chunk.Init();
                    _chunks.Add(chunk);
                }
            }
            
            // create chunks meshes
            foreach (var chunk in _chunks)
            {
                chunk.UpdateMesh();
            }
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
                if (VoxelCursor.Instance.VoxelChunk != null)
                {
                    var pos = VoxelCursor.Instance.BlockCoords;
                    VoxelCursor.Instance.VoxelChunk.SetBlock(0, pos.X, pos.Y, pos.Z);

                    if (VoxelChunk.IsOnEdge(pos))
                    {
                        // update neigh
                        var chunks = VoxelCursor.Instance.VoxelChunk.GetEdgeNeighs(pos);
                        if (chunks != null)
                        {
                            foreach (var chunk in chunks)
                            {
                                chunk?.UpdateMesh();
                            }
                        }
                    }

                    VoxelCursor.Instance.VoxelChunk.UpdateMesh();
                }
            }

            if (Input.IsButtonDown(ButtonCode.Right) && VoxelCursor.Instance.VoxelChunk != null)
            {
                if (VoxelCursor.Instance.VoxelChunk != null)
                {
                    var pos = VoxelCursor.Instance.BlockCoords;
                    var norm = VoxelCursor.Instance.Normal;

                    pos += new Int3((int) (1*norm.X), (int) (1*norm.Y), (int) (1*norm.Z));

                    if (VoxelCursor.Instance.VoxelChunk.GetBlock(pos.X, pos.Y, pos.Z) != 0)
                        return;

                    VoxelCursor.Instance.VoxelChunk.SetBlock(6, pos.X, pos.Y, pos.Z);

                    VoxelCursor.Instance.VoxelChunk.UpdateMesh();
                }
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

            foreach (var chunk in _chunks)
            {
                var pos = (Vector3)chunk.Position;

                Renderer.Instance.SetDepthTestState(false);
                Renderer.Instance.DrawBoundingBox(new BoundingBox(pos, pos + new Vector3(ChunkSize, ChunkHeight, ChunkSize)));
                Renderer.Instance.SetDepthTestState(true);
            }
        }

        /// <summary>
        /// Finds chunk which has the right position.
        /// </summary>
        /// <param name="position">The position.</param>
        /// <returns>The found chunk or null when not found.</returns>
        public VoxelChunk FindChunk(Int3 position)
        {
            return _chunks.FirstOrDefault(x => x.Position == position);
        }

        /// <summary>
        /// Finds chunk which contains the point.
        /// </summary>
        /// <param name="point">The point.</param>
        /// <returns>The found chunk or null when not found.</returns>
        public VoxelChunk FindChunk(Vector3 point)
        {
            return _chunks.FirstOrDefault(x => x.Bounds.Contains(point) == ContainmentType.Contains);
        }

        /// <summary>
        /// Find chunks which are intersecting the ray
        /// </summary>
        /// <param name="ray">The ray.</param>
        /// <returns>The intersecting chunks.</returns>
        public VoxelChunk[] FindChunksIntersecting(Ray ray)
        {
            return _chunks.Where(x => x.Bounds.Intersects(ref ray)).ToArray();
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
        /// <param name="normal">The hit normal.</param>
        public void RaycastBlocks(Vector3 origin, Vector3 direction, int distance, out ushort blockId, out Int3 coord, out VoxelChunk chunk, out Vector3 normal)
        {
            blockId = 0;
            coord = Int3.Zero;
            chunk = null;
            normal = Vector3.Up;
            
            const float precision = 200.0f;
            
            // for default 7 blocks distance there is 1400 iterations, TODO: needs some huge optimizations.
            for (var i = 0; i < distance * precision; i++)
            {
                var actualDistance = (i + 1) * (1 / precision);
                var position = origin + direction * actualDistance;
                
                var worldCoord = new Int3((int)position.X, (int)position.Y, (int)position.Z);
                
                var currentChunk = FindChunk((Vector3)worldCoord);

                if (currentChunk == null)
                    return;

                var chunkPos = currentChunk.Position;

                var blockCoord = new Int3(worldCoord.X - chunkPos.X, worldCoord.Y - chunkPos.Y, worldCoord.Z - chunkPos.Z);

                var block = currentChunk.GetBlock(blockCoord.X, blockCoord.Y, blockCoord.Z);
                
                if (block != 0)
                {
                    // calculate normal
                    var blockPos = new Vector3(worldCoord.X + 0.5f, worldCoord.Y + 0.5f, worldCoord.Z + 0.5f);
                    var dir = blockPos - position;
                    
                    if (Math.Abs(dir.X) > Math.Abs(dir.Y) && Math.Abs(dir.X) > Math.Abs(dir.Z))
                    {
                        if (dir.X > 0.1f)
                            normal = Vector3.Left;

                        if (dir.X < -0.1f)
                            normal = Vector3.Right;
                    }
                    else if (Math.Abs(dir.Y) > Math.Abs(dir.X) && Math.Abs(dir.Y) > Math.Abs(dir.Z))
                    {
                        if (dir.Y > 0.1f)
                            normal = Vector3.Down;

                        if (dir.Y < -0.1f)
                            normal = Vector3.Up;
                    }
                    else if (Math.Abs(dir.Z) > Math.Abs(dir.X) && Math.Abs(dir.Z) > Math.Abs(dir.Y))
                    {
                        if (dir.Z > 0.1f)
                            normal = Vector3.BackwardLH;

                        if (dir.Z < -0.1f)
                            normal = Vector3.ForwardLH;
                    }

                    // return data
                    chunk = currentChunk;
                    blockId = block;
                    
                    coord = blockCoord;
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