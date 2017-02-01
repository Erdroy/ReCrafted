// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

using System;
using System.Collections.Generic;
using System.Threading;
using ReCrafted.Core;
using ReCrafted.Graphics;
using SharpDX;
using ReCrafted.Voxels.Generator;

namespace ReCrafted.Voxels 
{
    /// <summary>
    /// VoxelChunk class.
    /// </summary>
    public sealed class VoxelChunk : IDisposable, IGameComponent
    {
        private static readonly Dictionary<uint, VoxelChunk> ChunkMeshUpdateQueue = new Dictionary<uint, VoxelChunk>();
        private static readonly List<uint> QueuedChunks = new List<uint>();
        private static uint _chunksUpdatesLastId;

        /// <summary>
        /// The neighbouring chunks.
        /// </summary>
        public VoxelChunk[] NeighChunks = new VoxelChunk[8];

        internal Dictionary<ushort, Mesh> _meshes = new Dictionary<ushort, Mesh>();
        internal readonly Dictionary<ushort, Mesh> MeshesWorker = new Dictionary<ushort, Mesh>();
        private ushort[,,] _voxels;
        private bool _isDone;

        /// <summary>
        /// Init the chunk.
        /// </summary>
        public void Init()
        {
            _voxels = new ushort[VoxelWorld.ChunkSize, VoxelWorld.ChunkHeight, VoxelWorld.ChunkSize];

            ThreadPool.QueueUserWorkItem(GenerateVoxels);
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
            
            if (Camera.Current.Position.X > Position.X 
                && Camera.Current.Position.Z > Position.Z 
                && Camera.Current.Position.X < Position.X + 16.0f
                && Camera.Current.Position.Z < Position.Z + 16.0f)
            {
                // camera is over this chunk.
                Current = this;
            }

            VoxelAssets.DefaultShader.Apply();
            VoxelAssets.DefaultShader.SetValue("WVP", wvp);
            VoxelAssets.DefaultShader.ApplyChanges();
            VoxelAssets.DefaultSampler.Apply(0);

            foreach (var mesh in _meshes)
            {
                if (mesh.Value.VerticeCount == 0)
                    continue;

                var block = VoxelAssets.Blocks[mesh.Key];

                // TODO: Use of custom shader
                VoxelAssets.DefaultShader.SetTexture(ShaderType.PS, 0, block.Texture);

                VoxelAssets.DefaultShader.Draw(mesh.Value);
            }
            VoxelAssets.DefaultShader.UnsetTexture(ShaderType.PS, 0);
        }
        
        /// <summary>
        /// Dispose the chunk.
        /// </summary>
        public void Dispose()
        {
            // TODO: valid chunk dispose
        }
        
        /// <summary>
        /// Update the chunk's mesh.
        /// </summary>
        public void UpdateMesh()
        {
            ChunkMeshUpdateQueue.Add(_chunksUpdatesLastId++, this);
        }

        /// <summary>
        /// Set block at given coordinates.
        /// This will rebuild the mesh!
        /// </summary>
        /// <param name="block">The block to be set, 0 - none(air, remove the block).</param>
        /// <param name="x">The x coordinate component.</param>
        /// <param name="y">The y coordinate component.</param>
        /// <param name="z">The z coordinate component.</param>
        public void SetBlock(ushort block, int x, int y, int z)
        {
            if (y >= VoxelWorld.ChunkHeight || y < 0 || x < 0 || z < 0 || x >= VoxelWorld.ChunkSize || z >= VoxelWorld.ChunkSize)
                return;
            
            _voxels[x, y, z] = block;
        }
        
        /// <summary>
        /// Get block from given coordinates.
        /// </summary>
        /// <param name="x">The x coordinate component.</param>
        /// <param name="y">The y coordinate component.</param>
        /// <param name="z">The z coordinate component.</param>
        /// <returns>The block(0 when none - air).</returns>
        public ushort GetBlock(int x, int y, int z)
        {
            if (y >= VoxelWorld.ChunkHeight || y < 0 || x < 0 || z < 0 || x >= VoxelWorld.ChunkSize || z >= VoxelWorld.ChunkSize)
                return 0;
            
            return _voxels[x, y, z];
        }

        /// <summary>
        /// Gets the neigh chunks, not including corners.
        /// </summary>
        /// <param name="localCoord">The local block coord.</param>
        /// <returns>The chunks.</returns>
        public VoxelChunk[] GetEdgeNeighs(Int3 localCoord)
        {
            // x--
            // |c|
            // ---
            if (localCoord.X == 0 && localCoord.Z == VoxelWorld.ChunkSize - 1)
            {
                return new[]
                {
                    NeighChunks[1],
                    NeighChunks[7],
                };
            }

            // --x
            // |c|
            // ---
            if (localCoord.X == VoxelWorld.ChunkSize - 1 && localCoord.Z == VoxelWorld.ChunkSize - 1)
            {
                return new[]
                {
                    NeighChunks[1],
                    NeighChunks[3],
                };
            }

            // ---
            // |c|
            // --x
            if (localCoord.X == VoxelWorld.ChunkSize - 1 && localCoord.Z == 0)
            {
                return new[]
                {
                    NeighChunks[3],
                    NeighChunks[5],
                };
            }

            // ---
            // |c|
            // x--
            if (localCoord.X == 0 && localCoord.Z == 0)
            {
                return new[]
                {
                    NeighChunks[5],
                    NeighChunks[7],
                };
            }

            // -x-
            // |c|
            // ---
            if (localCoord.X != VoxelWorld.ChunkSize - 1 && localCoord.Z == VoxelWorld.ChunkSize - 1)
            {
                return new[]
                {
                    NeighChunks[1]
                };
            }

            // ---
            // |cx
            // ---
            if (localCoord.X == VoxelWorld.ChunkSize - 1 && localCoord.Z != VoxelWorld.ChunkSize - 1)
            {
                return new[]
                {
                    NeighChunks[3]
                };
            }

            // ---
            // |c|
            // -x-
            if (localCoord.X != VoxelWorld.ChunkSize - 1 && localCoord.Z == 0)
            {
                return new[]
                {
                    NeighChunks[5]
                };
            }

            // ---
            // xc|
            // ---
            if (localCoord.X == 0 && localCoord.Z != VoxelWorld.ChunkSize - 1)
            {
                return new[]
                {
                    NeighChunks[7]
                };
            }

            return null;
        }

        /// <summary>
        /// Gets the neigh chunks, including corners.
        /// </summary>
        /// <param name="localCoord">The local block coord.</param>
        /// <returns>The chunks.</returns>
        public VoxelChunk[] GetEdgeNeighsWithCorners(Int3 localCoord)
        {
            // x--
            // |c|
            // ---
            if (localCoord.X == 0 && localCoord.Z == VoxelWorld.ChunkSize - 1)
            {
                return new[]
                {
                    NeighChunks[1],
                    NeighChunks[7],
                    NeighChunks[0]
                };
            }

            // --x
            // |c|
            // ---
            if (localCoord.X == VoxelWorld.ChunkSize - 1 && localCoord.Z == VoxelWorld.ChunkSize - 1)
            {
                return new[]
                {
                    NeighChunks[1],
                    NeighChunks[3],
                    NeighChunks[2]
                };
            }

            // ---
            // |c|
            // --x
            if (localCoord.X == VoxelWorld.ChunkSize - 1 && localCoord.Z == 0)
            {
                return new[]
                {
                    NeighChunks[3],
                    NeighChunks[5],
                    NeighChunks[4]
                };
            }

            // ---
            // |c|
            // x--
            if (localCoord.X == 0 && localCoord.Z == 0)
            {
                return new[]
                {
                    NeighChunks[5],
                    NeighChunks[7],
                    NeighChunks[6]
                };
            }

            // -x-
            // |c|
            // ---
            if (localCoord.X != VoxelWorld.ChunkSize - 1 && localCoord.Z == VoxelWorld.ChunkSize - 1)
            {
                return new[]
                {
                    NeighChunks[1]
                };
            }

            // ---
            // |cx
            // ---
            if (localCoord.X == VoxelWorld.ChunkSize - 1 && localCoord.Z != VoxelWorld.ChunkSize - 1)
            {
                return new[]
                {
                    NeighChunks[3]
                };
            }

            // ---
            // |c|
            // -x-
            if (localCoord.X != VoxelWorld.ChunkSize - 1 && localCoord.Z == 0)
            {
                return new[]
                {
                    NeighChunks[5]
                };
            }

            // ---
            // xc|
            // ---
            if (localCoord.X == 0 && localCoord.Z != VoxelWorld.ChunkSize - 1)
            {
                return new[]
                {
                    NeighChunks[7]
                };
            }

            return null;
        }

        // private
        private bool BlockExists(int x, int y, int z)
        {
            if (y >= VoxelWorld.ChunkHeight || y < 0)
                return false;

            if (x < 0 || z < 0 || x >= VoxelWorld.ChunkSize || z >= VoxelWorld.ChunkSize)
            {
                if (y < 0)
                {
                    return false;
                }

                if (y >= VoxelWorld.ChunkHeight)
                {
                    return false;
                }
                
                // -- corners --
                if (x < 0 && z >= VoxelWorld.ChunkSize)
                {
                    // x--
                    // |c|
                    // ---
                    var chunk = NeighChunks[0];

                    if (chunk == null)
                        return false;

                    return chunk.GetBlock(x + VoxelWorld.ChunkSize, y, z - VoxelWorld.ChunkSize) != 0;
                }
                if (z < 0 && x >= VoxelWorld.ChunkSize)
                {
                    // ---
                    // |c|
                    // --x
                    var chunk = NeighChunks[4];

                    if (chunk == null)
                        return false;

                    return chunk.GetBlock(x - VoxelWorld.ChunkSize, y, z + VoxelWorld.ChunkSize) != 0;
                }
                if (z >= VoxelWorld.ChunkSize && x >= VoxelWorld.ChunkSize)
                {
                    // --x
                    // |c|
                    // ---
                    var chunk = NeighChunks[2];

                    if (chunk == null)
                        return false;

                    return chunk.GetBlock(x - VoxelWorld.ChunkSize, y, z - VoxelWorld.ChunkSize) != 0;
                }
                if (x < 0 && z < 0)
                {
                    // ---
                    // |c|
                    // x--
                    var chunk = NeighChunks[6];

                    if (chunk == null)
                        return false;

                    return chunk.GetBlock(x + VoxelWorld.ChunkSize, y, z + VoxelWorld.ChunkSize) != 0;
                }

                // -- sides --
                if (z >= VoxelWorld.ChunkSize)
                {
                    // -x-
                    // |c|
                    // ---
                    var chunk = NeighChunks[1];

                    if (chunk == null)
                        return false;

                    return chunk.GetBlock(x, y, z - VoxelWorld.ChunkSize) != 0;
                }
                if (x >= VoxelWorld.ChunkSize)
                {
                    // ---
                    // |cx
                    // ---
                    var chunk = NeighChunks[3];

                    if (chunk == null)
                        return false;

                    return chunk.GetBlock(x - VoxelWorld.ChunkSize, y, z) != 0;
                }
                if (z < 0)
                {
                    // ---
                    // |c|
                    // -x-
                    var chunk = NeighChunks[5];

                    if (chunk == null)
                        return false;

                    return chunk.GetBlock(x, y, z + VoxelWorld.ChunkSize) != 0;
                } 
                if (x < 0)
                {
                    // ---
                    // xc|
                    // ---
                    var chunk = NeighChunks[7];

                    if (chunk == null)
                        return false;

                    return chunk.GetBlock(x + VoxelWorld.ChunkSize, y, z) != 0;
                }
                
                return false; // TODO: Check in the neigh chunk
            }

            return _voxels[x, y, z] != 0;
        }

        // private
        private void UpdateNeighs()
        {
            // x--
            // |c|
            // ---
            var c0 = Position + new Int3(-VoxelWorld.ChunkSize, 0, VoxelWorld.ChunkSize);

            // -x-
            // |c|
            // ---
            var c1 = Position + new Int3(0, 0, VoxelWorld.ChunkSize);

            // --x
            // |c|
            // ---
            var c2 = Position + new Int3(VoxelWorld.ChunkSize, 0, VoxelWorld.ChunkSize);

            // ---
            // |cx
            // ---
            var c3 = Position + new Int3(VoxelWorld.ChunkSize, 0, 0);

            // ---
            // |c|
            // --x
            var c4 = Position + new Int3(VoxelWorld.ChunkSize, 0, -VoxelWorld.ChunkSize);

            // ---
            // |c|
            // -x-
            var c5 = Position + new Int3(0, 0, -VoxelWorld.ChunkSize);

            // ---
            // |c|
            // x--
            var c6 = Position + new Int3(-VoxelWorld.ChunkSize, 0, -VoxelWorld.ChunkSize);

            // ---
            // xc|
            // ---
            var c7 = Position + new Int3(-VoxelWorld.ChunkSize, 0, 0);

            // find chunks
            NeighChunks[0] = VoxelWorld.Instance.FindChunk(c0);
            NeighChunks[1] = VoxelWorld.Instance.FindChunk(c1);
            NeighChunks[2] = VoxelWorld.Instance.FindChunk(c2);
            NeighChunks[3] = VoxelWorld.Instance.FindChunk(c3);
            NeighChunks[4] = VoxelWorld.Instance.FindChunk(c4);
            NeighChunks[5] = VoxelWorld.Instance.FindChunk(c5);
            NeighChunks[6] = VoxelWorld.Instance.FindChunk(c6);
            NeighChunks[7] = VoxelWorld.Instance.FindChunk(c7);
        }

        // private
        private void UpdateMeshWorker(object obj)
        {
            UpdateNeighs();

            var blocksVertices = new Dictionary<ushort, List<Vector3>>();
            var blocksIndices = new Dictionary<ushort, List<uint>>();
            var blocksUvs = new Dictionary<ushort, List<Vector2>>();
            var blocksNormals = new Dictionary<ushort, List<Vector3>>();
            var blocksColors = new Dictionary<ushort, List<Vector4>>();

            for (var z = 0; z < VoxelWorld.ChunkSize; z++)
            {
                for (var y = 0; y < VoxelWorld.ChunkHeight; y++)
                {
                    for (var x = 0; x < VoxelWorld.ChunkSize; x++)
                    {
                        var block = _voxels[x, y, z];

                        if (block == 0)
                            continue;

                        if (!blocksVertices.ContainsKey(block))
                        {
                            blocksVertices.Add(block, new List<Vector3>());
                            blocksIndices.Add(block, new List<uint>());
                            blocksUvs.Add(block, new List<Vector2>());
                            blocksNormals.Add(block, new List<Vector3>());
                            blocksColors.Add(block, new List<Vector4>());
                        }

                        var vertices = blocksVertices[block];
                        var indices = blocksIndices[block];
                        var uvs = blocksUvs[block];
                        var normals = blocksNormals[block];
                        var colors = blocksColors[block];

                        var origin = new Vector3(x, y, z);


                        var tf = BlockExists(x, y + 1, z + 1);
                        var tb = BlockExists(x, y + 1, z - 1);
                        var tl = BlockExists(x - 1, y + 1, z);
                        var tr = BlockExists(x + 1, y + 1, z);

                        var tfl = BlockExists(x - 1, y + 1, z + 1);
                        var tfr = BlockExists(x + 1, y + 1, z + 1);
                        var tbl = BlockExists(x - 1, y + 1, z - 1);
                        var tbr = BlockExists(x + 1, y + 1, z - 1);

                        var mfl = BlockExists(x - 1, y, z + 1);
                        var mfr = BlockExists(x + 1, y, z + 1);
                        var mbl = BlockExists(x - 1, y, z - 1);
                        var mbr = BlockExists(x + 1, y, z - 1);

                        var bf = BlockExists(x, y - 1, z + 1);
                        var bb = BlockExists(x, y - 1, z - 1);
                        var bl = BlockExists(x - 1, y - 1, z);
                        var br = BlockExists(x + 1, y - 1, z);

                        var bfl = BlockExists(x - 1, y - 1, z + 1);
                        var bfr = BlockExists(x + 1, y - 1, z + 1);
                        var bbl = BlockExists(x - 1, y - 1, z - 1);
                        var bbr = BlockExists(x + 1, y - 1, z - 1);

                        const float ambientStr = 0.45f;

                        // left face
                        if (!BlockExists(x - 1, y, z))
                        {
                            var ao00 = VoxelMeshHelper.CalculateAmbient(mfl, bl, bfl);
                            var ao01 = VoxelMeshHelper.CalculateAmbient(mfl, tl, tfl);
                            var ao11 = VoxelMeshHelper.CalculateAmbient(mbl, tl, tbl);
                            var ao10 = VoxelMeshHelper.CalculateAmbient(mbl, bl, bbl);

                            var flippedQuad = ao01 + ao10 > ao00 + ao11;

                            VoxelMeshHelper.SetupFace(
                                 origin + new Vector3(0.0f, 0.0f, 0.0f),
                                 Vector3.Up * VoxelWorld.BlockSize, Vector3.ForwardLH * VoxelWorld.BlockSize, false, flippedQuad, vertices, uvs, indices, normals);

                            colors.Add(new Vector4(ao10 * ambientStr, 0.0f, 0.0f, 0.0f));
                            colors.Add(new Vector4(ao11 * ambientStr, 0.0f, 0.0f, 0.0f));
                            colors.Add(new Vector4(ao01 * ambientStr, 0.0f, 0.0f, 0.0f));
                            colors.Add(new Vector4(ao00 * ambientStr, 0.0f, 0.0f, 0.0f));
                        }

                        // right face
                        if (!BlockExists(x + 1, y, z))
                        {
                            var ao00 = VoxelMeshHelper.CalculateAmbient(br, mbr, bbr);
                            var ao01 = VoxelMeshHelper.CalculateAmbient(tr, mbr, tbr);
                            var ao11 = VoxelMeshHelper.CalculateAmbient(tr, mfr, tfr);
                            var ao10 = VoxelMeshHelper.CalculateAmbient(br, mfr, bfr);

                            var flippedQuad = ao00 + ao11 > ao01 + ao10;

                            VoxelMeshHelper.SetupFace(
                                origin + new Vector3(VoxelWorld.BlockSize, 0.0f, 0.0f),
                                Vector3.Up * VoxelWorld.BlockSize, Vector3.ForwardLH * VoxelWorld.BlockSize, true, flippedQuad, vertices, uvs, indices, normals);

                            colors.Add(new Vector4(ao00 * ambientStr, 0.0f, 0.0f, 0.0f));
                            colors.Add(new Vector4(ao01 * ambientStr, 0.0f, 0.0f, 0.0f));
                            colors.Add(new Vector4(ao11 * ambientStr, 0.0f, 0.0f, 0.0f));
                            colors.Add(new Vector4(ao10 * ambientStr, 0.0f, 0.0f, 0.0f));
                        }

                        // bottom face
                        if (!BlockExists(x, y - 1, z))
                        {
                            var ao00 = VoxelMeshHelper.CalculateAmbient(bb, bl, bbl);
                            var ao01 = VoxelMeshHelper.CalculateAmbient(bf, bl, bfl);
                            var ao11 = VoxelMeshHelper.CalculateAmbient(bf, br, bfr);
                            var ao10 = VoxelMeshHelper.CalculateAmbient(bb, br, bbr);

                            var flippedQuad = ao00 + ao11 > ao01 + ao10;

                            VoxelMeshHelper.SetupFace(
                               origin + new Vector3(0.0f, 0.0f, 0.0f),
                               Vector3.ForwardLH * VoxelWorld.BlockSize, Vector3.Right * VoxelWorld.BlockSize, false, flippedQuad, vertices, uvs, indices, normals);

                            colors.Add(new Vector4(ao00 * ambientStr, 0.0f, 0.0f, 0.0f)); // ?
                            colors.Add(new Vector4(ao01 * ambientStr, 0.0f, 0.0f, 0.0f)); // ?
                            colors.Add(new Vector4(ao11 * ambientStr, 0.0f, 0.0f, 0.0f)); // ? 
                            colors.Add(new Vector4(ao10 * ambientStr, 0.0f, 0.0f, 0.0f)); // ?
                        }

                        // upper face
                        if (!BlockExists(x, y + 1, z))
                        {
                            var ao00 = VoxelMeshHelper.CalculateAmbient(tl, tb, tbl);
                            var ao01 = VoxelMeshHelper.CalculateAmbient(tl, tf, tfl);
                            var ao11 = VoxelMeshHelper.CalculateAmbient(tr, tf, tfr);
                            var ao10 = VoxelMeshHelper.CalculateAmbient(tb, tr, tbr);

                            var flippedQuad = ao00 + ao11 > ao01 + ao10;

                            VoxelMeshHelper.SetupFace(
                                origin + new Vector3(0.0f, VoxelWorld.BlockSize, 0.0f),
                                Vector3.ForwardLH * VoxelWorld.BlockSize, Vector3.Right * VoxelWorld.BlockSize, true, flippedQuad, vertices, uvs, indices, normals);

                            colors.Add(new Vector4(ao00 * ambientStr, 0.0f, 0.0f, 0.0f)); // bl
                            colors.Add(new Vector4(ao01 * ambientStr, 0.0f, 0.0f, 0.0f)); // ul
                            colors.Add(new Vector4(ao11 * ambientStr, 0.0f, 0.0f, 0.0f)); // ur
                            colors.Add(new Vector4(ao10 * ambientStr, 0.0f, 0.0f, 0.0f)); // br
                        }

                        // back face
                        if (!BlockExists(x, y, z - 1))
                        {
                            var ao00 = VoxelMeshHelper.CalculateAmbient(bb, mbl, bbl);
                            var ao01 = VoxelMeshHelper.CalculateAmbient(mbl, tb, tbl);
                            var ao11 = VoxelMeshHelper.CalculateAmbient(mbr, tb, tbr);
                            var ao10 = VoxelMeshHelper.CalculateAmbient(mbr, bb, bbr);

                            var flippedQuad = ao00 + ao11 > ao01 + ao10;

                            VoxelMeshHelper.SetupFace(
                                origin + new Vector3(0.0f, 0.0f, 0.0f),
                                Vector3.Up * VoxelWorld.BlockSize, Vector3.Right * VoxelWorld.BlockSize, true, flippedQuad, vertices, uvs, indices, normals);

                            colors.Add(new Vector4(ao00 * ambientStr, 0.0f, 0.0f, 0.0f)); // ll
                            colors.Add(new Vector4(ao01 * ambientStr, 0.0f, 0.0f, 0.0f)); // ul
                            colors.Add(new Vector4(ao11 * ambientStr, 0.0f, 0.0f, 0.0f)); // ur
                            colors.Add(new Vector4(ao10 * ambientStr, 0.0f, 0.0f, 0.0f)); // lr
                        }

                        // front face
                        if (!BlockExists(x, y, z + 1))
                        {
                            var ao00 = VoxelMeshHelper.CalculateAmbient(mfl, bf, bfl);
                            var ao01 = VoxelMeshHelper.CalculateAmbient(mfl, tf, tfl);
                            var ao11 = VoxelMeshHelper.CalculateAmbient(mfr, tf, tfr);
                            var ao10 = VoxelMeshHelper.CalculateAmbient(mfr, bf, bfr);

                            var flippedQuad = ao00 + ao11 > ao01 + ao10;

                            VoxelMeshHelper.SetupFace(
                                origin + new Vector3(0.0f, 0.0f, VoxelWorld.BlockSize),
                                Vector3.Up * VoxelWorld.BlockSize, Vector3.Right * VoxelWorld.BlockSize, false, flippedQuad, vertices, uvs, indices, normals);

                            colors.Add(new Vector4(ao00 * ambientStr, 0.0f, 0.0f, 0.0f));
                            colors.Add(new Vector4(ao01 * ambientStr, 0.0f, 0.0f, 0.0f));
                            colors.Add(new Vector4(ao11 * ambientStr, 0.0f, 0.0f, 0.0f));
                            colors.Add(new Vector4(ao10 * ambientStr, 0.0f, 0.0f, 0.0f));
                        }
                    }
                }
            }
            
            // clear
            MeshesWorker.Clear();

            foreach (var vertices in blocksVertices)
            {
                var verts = vertices.Value.ToArray();

                if (verts.Length > 0)
                {
                    var mesh = Mesh.Create();

                    var uvs = blocksUvs[vertices.Key].ToArray();
                    var normals = blocksNormals[vertices.Key].ToArray();
                    var colors = blocksColors[vertices.Key].ToArray();

                    var indices = blocksIndices[vertices.Key].ToArray();

                    mesh.SetVertices(verts);
                    mesh.SetUVs(uvs);
                    mesh.SetNormals(normals);
                    mesh.SetColors(colors);

                    mesh.SetIndices(indices);

                    MeshesWorker.Add(vertices.Key, mesh);
                }
            }

            _isDone = true;
            IsVisible = true;
        }

        // private
        private void GenerateVoxels(object obj)
        {
            for (var z = 0; z < VoxelWorld.ChunkSize; z++)
            {
                for (var y = 0; y < VoxelWorld.ChunkHeight; y++)
                {
                    for (var x = 0; x < VoxelWorld.ChunkSize; x++)
                    {
                        if (y == 0)
                        {
                            _voxels[x, y, z] = 5;
                            continue;
                        }

                        // generate terrain
                        _voxels[x, y, z] = VoxelWorldGenerator.GenerateTerrains(new Int3(x, y, z) + Position);

                        // generate rivers, oceans, ponds etc.

                        // generate caves
                        _voxels[x, y, z] = VoxelWorldGenerator.GenerateCaves(_voxels[x, y, z], new Int3(x, y, z) + Position);

                        //if (_voxels[x, y, z] == 1)
                        //    _voxels[x, y, z] = 0;
                    }
                }
            }

            // generate structures(buildings, trees)

            IsLoaded = true;
        }

        /// <summary>
        /// Check if local coordinate is on chunk edge.
        /// </summary>
        /// <param name="localCoord">The local coord.</param>
        /// <returns>True when coord in on edge.</returns>
        public static bool IsOnEdge(Int3 localCoord)
        {
            return localCoord.X == 0
                   || localCoord.Y == 0
                   || localCoord.Z == 0
                   || localCoord.X == VoxelWorld.ChunkSize - 1
                   || localCoord.Y == VoxelWorld.ChunkHeight - 1
                   || localCoord.Z == VoxelWorld.ChunkSize - 1;
        }

        /// <summary>
        /// Check if local coordinate is on chunk edge.
        /// </summary>
        /// <param name="localCoord">The local coord.</param>
        /// <returns>True when coord in on edge.</returns>
        public static bool IsInCorner(Int3 localCoord)
        {
            return
                   (localCoord.X == 0 && localCoord.Z == VoxelWorld.ChunkSize - 1)
                   || (localCoord.X == VoxelWorld.ChunkSize - 1 && localCoord.Z == VoxelWorld.ChunkSize - 1)
                   || (localCoord.X == VoxelWorld.ChunkSize - 1 && localCoord.Z == 0)
                   || (localCoord.X == 0 && localCoord.Z == 0);
        }

        public static void TickChunkLoader()
        {
            var doneChunks = 0;
            foreach (var chunkid in QueuedChunks)
            {
                if (ChunkMeshUpdateQueue[chunkid]._isDone)
                    doneChunks++;
            }

            if (doneChunks == QueuedChunks.Count)
            {
                foreach (var chunkid in QueuedChunks)
                {
                    var chunk = ChunkMeshUpdateQueue[chunkid];

                    foreach (var mesh in chunk._meshes)
                    {
                        mesh.Value?.Dispose();
                    }
                    chunk._meshes.Clear();

                    foreach (var mesh in chunk.MeshesWorker)
                    {
                        chunk._meshes.Add(mesh.Key, mesh.Value);
                    }

                    foreach (var mesh in chunk._meshes)
                    {
                        if (mesh.Value != null && mesh.Value.HasVertices)
                            mesh.Value.ApplyChanges();
                    }

                    ChunkMeshUpdateQueue.Remove(chunkid);
                }

                QueuedChunks.Clear();
            }
            else
                return;

            if (ChunkMeshUpdateQueue.Count == 0)
                return;
            
            foreach (var item in ChunkMeshUpdateQueue)
            {
                if (!item.Value.IsLoaded)
                    continue;

                QueuedChunks.Add(item.Key);

                // run
                ThreadPool.QueueUserWorkItem(item.Value.UpdateMeshWorker);

                if (QueuedChunks.Count == 4)
                    return;
            }
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

        public VoxelChunk Front => NeighChunks[1];

        public VoxelChunk Back => NeighChunks[5];

        public VoxelChunk Left => NeighChunks[7];
        
        public VoxelChunk Right => NeighChunks[3];

        /// <summary>
        /// The chunk bounds(w/o 1 block edge).
        /// </summary>
        public BoundingBox Bounds
        {
            get
            {
                var pos = new Vector3(Position.X, Position.Y, Position.Z);
                return new BoundingBox(pos, pos + new Vector3(VoxelWorld.ChunkSize - 1, VoxelWorld.ChunkHeight - 1, VoxelWorld.ChunkSize - 1));
            }
        }

        /// <summary>
        /// The real chunk bounds.
        /// </summary>
        public BoundingBox RealBounds
        {
            get
            {
                var pos = new Vector3(Position.X, Position.Y, Position.Z);
                return new BoundingBox(pos, pos + new Vector3(VoxelWorld.ChunkSize, VoxelWorld.ChunkHeight, VoxelWorld.ChunkSize));
            }
        }

        public static VoxelChunk Current { get; private set; }
    }
}