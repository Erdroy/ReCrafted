// ReCrafted © 2016 Damian 'Erdroy' Korczowski

using System;
using System.Collections.Generic;
using ReCrafted.Core;
using ReCrafted.Graphics;
using SharpDX;

namespace ReCrafted.Voxels 
{
    /// <summary>
    /// VoxelChunk class.
    /// </summary>
    public sealed class VoxelChunk : IDisposable, IRendererComponent
    {
        /// <summary>
        /// The block size.
        /// </summary>
        public const float BlockSize = 1.0f;

        private readonly Dictionary<ushort, Mesh> _meshes = new Dictionary<ushort, Mesh>();
        
        private ushort[,,] _voxels;

        /// <summary>
        /// Init the chunk.
        /// </summary>
        public void Init()
        {
            _voxels = new ushort[16, 256, 16];

            var random = new Random();

            for (var z = 0; z < 16; z++)
            {
                for (var y = 0; y < 256; y++)
                {
                    for (var x = 0; x < 16; x++)
                    {
                        if (y == 0)
                        {
                            _voxels[x, y, z] = 5;
                        }
                        else if (y < 4)
                        {
                            var block = random.Next(0, 3) == 0 ? (ushort)1 : (ushort)2;

                            _voxels[x, y, z] = random.Next(0, 3) == 1 ? block : (ushort)0;
                        }
                        else
                        {
                            _voxels[x, y, z] = 0;
                        }
                    }
                }
            }
        }

        /// <summary>
        /// Tick the chunk.
        /// </summary>
        public void Tick()
        {
        }

        /// <summary>
        /// Draw the chunk.
        /// </summary>
        public void Draw()
        {
            if (_meshes.Count == 0)
                return;

            var wvp = Matrix.Translation(new Vector3(Position.X, Position.Y, Position.Z)) * Camera.Current.ViewProjectionMatrix;
            wvp.Transpose();
            
            foreach (var mesh in _meshes)
            {
                var block = VoxelAssets.Blocks[mesh.Key];


                if (block.CustomShader == null)
                {
                    VoxelAssets.DefaultShader.SetValue("WVP", wvp);
                }
                else
                {
                    // TODO: Use of custom shader
                }

                block.Texture.Apply(0);
                VoxelAssets.DefaultSampler.Apply(0);

                VoxelAssets.DefaultShader.Draw(mesh.Value);
            }
        }
        
        /// <summary>
        /// Dispose the chunk.
        /// </summary>
        public void Dispose()
        {
            foreach (var mesh in _meshes)
            {
                mesh.Value.Dispose();
            }
            _meshes.Clear();
        }
        
        /// <summary>
        /// Update the chunk's mesh.
        /// </summary>
        public void UpdateMesh()
        {
            var blocksVertices = new Dictionary<ushort, List<Vector3>>();
            var blocksIndices = new Dictionary<ushort, List<uint>>();
            var blocksUvs = new Dictionary<ushort, List<Vector2>>();

            for (var z = 0; z < 16; z++)
            {
                for (var y = 0; y < 256; y++)
                {
                    for (var x = 0; x < 16; x++)
                    {
                        var block = _voxels[x, y, z];
                        
                        if (block == 0)
                            continue;

                        if (!blocksVertices.ContainsKey(block))
                        {
                            blocksVertices.Add(block, new List<Vector3>());
                            blocksIndices.Add(block, new List<uint>());
                            blocksUvs.Add(block, new List<Vector2>());
                        }

                        var vertices = blocksVertices[block];
                        var indices = blocksIndices[block];
                        var uvs = blocksUvs[block];

                        var origin = new Vector3(x, y, z);
                        
                        if(!BlockExists(x-1, y, z))
                            VoxelMeshHelper.SetupFace(origin + new Vector3(0.0f, 0.0f, 0.0f), Vector3.Up * BlockSize, Vector3.ForwardLH * BlockSize, false, vertices, uvs, indices);
                        if (!BlockExists(x+1, y, z))
                            VoxelMeshHelper.SetupFace(origin + new Vector3(BlockSize, 0.0f, 0.0f), Vector3.Up * BlockSize, Vector3.ForwardLH * BlockSize, true, vertices, uvs, indices);

                        if (!BlockExists(x, y-1, z))
                            VoxelMeshHelper.SetupFace(origin + new Vector3(0.0f, 0.0f, 0.0f), Vector3.ForwardLH * BlockSize, Vector3.Right * BlockSize, false, vertices, uvs, indices);
                        if (!BlockExists(x, y+1, z))
                            VoxelMeshHelper.SetupFace(origin + new Vector3(0.0f, BlockSize, 0.0f), Vector3.ForwardLH * BlockSize, Vector3.Right * BlockSize, true, vertices, uvs, indices);

                        if (!BlockExists(x, y, z-1))
                            VoxelMeshHelper.SetupFace(origin + new Vector3(0.0f, 0.0f, 0.0f), Vector3.Up * BlockSize, Vector3.Right * BlockSize, true, vertices, uvs, indices);
                        if (!BlockExists(x, y, z+1))
                            VoxelMeshHelper.SetupFace(origin + new Vector3(0.0f, 0.0f, BlockSize), Vector3.Up * BlockSize, Vector3.Right * BlockSize, false, vertices, uvs, indices);
                    }
                }
            }

            foreach (var vertices in blocksVertices)
            {
                var mesh = Mesh.Create();

                var verts = vertices.Value.ToArray();
                var indices = blocksIndices[vertices.Key].ToArray();
                var uvs = blocksUvs[vertices.Key].ToArray();

                mesh.SetVertices(verts);
                mesh.SetIndices(indices);
                mesh.SetUVs(uvs);

                if (verts.Length > 0)
                {
                    mesh.ApplyChanges();
                }

                _meshes.Add(vertices.Key, mesh);
            }
        }

        // private
        private bool BlockExists(int x, int y, int z)
        {
            if (y >= 256 || y < 0)
                return false;

            if (x < 0 || z < 0 || x >= 16 || z >= 16)
                return false; // TODO: Check in the neigh chunk

            return _voxels[x, y, z] != 0;
        }
        
        /// <summary>
        /// The Chunk position.
        /// </summary>
        public Int3 Position { get; set; }

        /// <summary>
        /// Is this chunk loaded?
        /// </summary>
        public bool IsLoaded { get; set; }

        /// <summary>
        /// Is this chunk visible?
        /// </summary>
        public bool IsVisible { get; set; }
    }
}