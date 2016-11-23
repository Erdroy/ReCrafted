using System;
using System.Collections.Generic;
using ReCrafted.Core;
using ReCrafted.Graphics;
using SharpDX;

namespace ReCrafted.Voxels 
{
    public class VoxelChunk : IDisposable, IRendererComponent
    {
        public const float BlockSize = 1.0f;

        private Mesh _mesh;
        private byte[,,] _voxels;

        public void Init()
        {
            _mesh = Mesh.Create();
            _voxels = new byte[16,256,16];

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
                            _voxels[x, y, z] = random.Next(0, 3) == 1 ? (byte)1 : (byte)0;
                        }
                        else
                        {
                            _voxels[x, y, z] = 0;
                        }
                    }
                }
            }
            
            UpdateMesh();
        }

        public void Tick()
        {
        }

        public void Draw()
        {
            if (_mesh.Vertices.Length == 0)
                return;

            var wvp = Matrix.Translation(Position) * Camera.Current.ViewProjectionMatrix;
            wvp.Transpose();

            VoxelAssets.DefaultShader.SetValue("WVP", wvp);
            VoxelAssets.DefaultShader.Draw(_mesh);
        }

        public void Dispose()
        {
            _mesh.Dispose();
        }

        // private
        private void UpdateMesh()
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

                        // TODO: check block visibility

                        VoxelMeshHelper.SetupFace(origin + new Vector3(0.0f, 0.0f, 0.0f), Vector3.Up * BlockSize, Vector3.ForwardLH * BlockSize, false, vertices, uvs, indices);
                        VoxelMeshHelper.SetupFace(origin + new Vector3(BlockSize, 0.0f, 0.0f), Vector3.Up * BlockSize, Vector3.ForwardLH * BlockSize, true, vertices, uvs, indices);

                        VoxelMeshHelper.SetupFace(origin + new Vector3(0.0f, 0.0f, 0.0f), Vector3.ForwardLH * BlockSize, Vector3.Right * BlockSize, false, vertices, uvs, indices);
                        VoxelMeshHelper.SetupFace(origin + new Vector3(0.0f, BlockSize, 0.0f), Vector3.ForwardLH * BlockSize, Vector3.Right * BlockSize, true, vertices, uvs, indices);

                        VoxelMeshHelper.SetupFace(origin + new Vector3(0.0f, 0.0f, 0.0f), Vector3.Up * BlockSize, Vector3.Right * BlockSize, true, vertices, uvs, indices);
                        VoxelMeshHelper.SetupFace(origin + new Vector3(0.0f, 0.0f, BlockSize), Vector3.Up * BlockSize, Vector3.Right * BlockSize, false, vertices, uvs, indices);
                    }
                }
            }

            _mesh.SetVertices(vertices.ToArray());
            _mesh.SetIndices(indices.ToArray());

            if (vertices.Count > 0)
            {
                _mesh.ApplyChanges();
            }
        }
        

        public Vector3 Position { get; set; }
    }
}