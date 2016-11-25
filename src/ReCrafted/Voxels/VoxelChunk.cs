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

        private Mesh _mesh;
        private ushort[,,] _voxels;
        
        /// <summary>
        /// Init the chunk.
        /// </summary>
        public void Init()
        {
            _mesh = Mesh.Create();
            _voxels = new ushort[16,256,16];

            var random = new Random();

            for (var z = 0; z < 16; z++)
            {
                for (var y = 0; y < 256; y++)
                {
                    for (var x = 0; x < 16; x++)
                    {
                        if (y == 0)
                        {
                            _voxels[x, y, z] = 1;
                        }
                        else if (y < 4)
                        {
                            _voxels[x, y, z] = random.Next(0, 3) == 1 ? (ushort)1 : (ushort)0;
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
            if (_mesh.Vertices.Length == 0)
                return;

            var wvp = Matrix.Translation(new Vector3(Position.X, Position.Y, Position.Z)) * Camera.Current.ViewProjectionMatrix;
            wvp.Transpose();

            VoxelAssets.DefaultSampler.Apply(0);
            VoxelAssets.DefaultShader.SetTexture(0, VoxelAssets.DefaultAtlas);

            VoxelAssets.DefaultShader.SetValue("WVP", wvp);
            VoxelAssets.DefaultShader.Draw(_mesh);
        }
        
        /// <summary>
        /// Dispose the chunk.
        /// </summary>
        public void Dispose()
        {
            _mesh.Dispose();
        }
        
        /// <summary>
        /// Update the chunk's mesh.
        /// </summary>
        public void UpdateMesh()
        {
            var vertices = new List<Vector3>();
            var indices = new List<uint>();
            var uvs = new List<Vector2>();

            for (var z = 0; z < 16; z++)
            {
                for (var y = 0; y < 256; y++)
                {
                    for (var x = 0; x < 16; x++)
                    {
                        var block = _voxels[x, y, z];

                        if (block == 0)
                            continue;

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

            _mesh.SetVertices(vertices.ToArray());
            _mesh.SetIndices(indices.ToArray());
            _mesh.SetUVs(uvs.ToArray());

            if (vertices.Count > 0)
            {
                _mesh.ApplyChanges();
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