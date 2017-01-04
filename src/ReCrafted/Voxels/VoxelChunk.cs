// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

using System;
using System.Collections.Generic;
using ReCrafted.Core;
using ReCrafted.Graphics;
using SharpDX;
using ReCrafted.Voxels.Generator;

namespace ReCrafted.Voxels 
{
    /// <summary>
    /// VoxelChunk class.
    /// </summary>
    public sealed class VoxelChunk : IDisposable, IRendererComponent
    {
        /// <summary>
        /// The neighbouring chunks.
        /// </summary>
        public VoxelChunk[] NeighChunks = new VoxelChunk[8];

        internal readonly Dictionary<ushort, Mesh> Meshes = new Dictionary<ushort, Mesh>();
        private ushort[,,] _voxels;

        /// <summary>
        /// Init the chunk.
        /// </summary>
        public void Init()
        {
            _voxels = new ushort[VoxelWorld.ChunkSize, VoxelWorld.ChunkHeight, VoxelWorld.ChunkSize];
            
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
            if (Meshes.Count == 0)
                return;

            var wvp = Matrix.Translation(new Vector3(Position.X, Position.Y, Position.Z)) * Camera.Current.ViewProjectionMatrix;
            wvp.Transpose();

            VoxelAssets.DefaultShader.Apply();

            foreach (var mesh in Meshes)
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
                VoxelAssets.DefaultShader.SetTexture(ShaderType.PS, 0, block.Texture);
                VoxelAssets.DefaultSampler.Apply(0);

                VoxelAssets.DefaultShader.ApplyChanges();
                VoxelAssets.DefaultShader.Draw(mesh.Value);
                VoxelAssets.DefaultShader.UnsetTexture(ShaderType.PS, 0);
            }
        }
        
        /// <summary>
        /// Dispose the chunk.
        /// </summary>
        public void Dispose()
        {
            foreach (var mesh in Meshes)
            {
                mesh.Value.Dispose();
            }
            Meshes.Clear();
        }
        
        /// <summary>
        /// Update the chunk's mesh.
        /// </summary>
        public void UpdateMesh()
        {
            UpdateNeighs();
            
            // clear
            foreach (var mesh in Meshes)
            {
                mesh.Value.Dispose();
            }
            Meshes.Clear();

            var blocksVertices = new Dictionary<ushort, List<Vector3>>();
            var blocksIndices = new Dictionary<ushort, List<uint>>();
            var blocksUvs = new Dictionary<ushort, List<Vector2>>();
            var blocksNormals = new Dictionary<ushort, List<Vector3>>();
            var blocksColors = new Dictionary<ushort, List<Color>>();

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
                            blocksColors.Add(block, new List<Color>());
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

                        // left face
                        if (!BlockExists(x-1, y, z))
                        {
                            VoxelMeshHelper.SetupFace(
                                 origin + new Vector3(0.0f, 0.0f, 0.0f),
                                 Vector3.Up * VoxelWorld.BlockSize, Vector3.ForwardLH * VoxelWorld.BlockSize, false, false, vertices, uvs, indices, normals);

                            colors.Add(new Color(0.0f, 0.0f, 0.0f, 0.0f));
                            colors.Add(new Color(0.0f, 0.0f, 0.0f, 0.0f));
                            colors.Add(new Color(0.0f, 0.0f, 0.0f, 0.0f));
                            colors.Add(new Color(0.0f, 0.0f, 0.0f, 0.0f));
                        }

                        // right face
                        if (!BlockExists(x + 1, y, z))
                        {
                            VoxelMeshHelper.SetupFace(
                                origin + new Vector3(VoxelWorld.BlockSize, 0.0f, 0.0f),
                                Vector3.Up * VoxelWorld.BlockSize, Vector3.ForwardLH * VoxelWorld.BlockSize, true, false, vertices, uvs, indices, normals);

                            var ao00 = VoxelMeshHelper.CalculateAmbient(br, mbr, bbr);
                            var ao01 = VoxelMeshHelper.CalculateAmbient(tr, mbr, tbr);
                            var ao11 = VoxelMeshHelper.CalculateAmbient(tr, mfr, tfr);
                            var ao10 = VoxelMeshHelper.CalculateAmbient(br, mbr, bfr);

                            colors.Add(new Color(ao00 * 0.44f, 0.0f, 0.0f, 0.0f));
                            colors.Add(new Color(ao01 * 0.44f, 0.0f, 0.0f, 0.0f));
                            colors.Add(new Color(ao11 * 0.44f, 0.0f, 0.0f, 0.0f));
                            colors.Add(new Color(ao10 * 0.44f, 0.0f, 0.0f, 0.0f));
                        }

                        // bottom face
                        if (!BlockExists(x, y - 1, z))
                        {
                            VoxelMeshHelper.SetupFace(
                               origin + new Vector3(0.0f, 0.0f, 0.0f),
                               Vector3.ForwardLH * VoxelWorld.BlockSize, Vector3.Right * VoxelWorld.BlockSize, false, false, vertices, uvs, indices, normals);

                            colors.Add(new Color(0.0f, 0.0f, 0.0f, 0.0f));
                            colors.Add(new Color(0.0f, 0.0f, 0.0f, 0.0f));
                            colors.Add(new Color(0.0f, 0.0f, 0.0f, 0.0f));
                            colors.Add(new Color(0.0f, 0.0f, 0.0f, 0.0f));
                        }

                        // upper face
                        if (!BlockExists(x, y + 1, z))
                        {
                            VoxelMeshHelper.SetupFace(
                                origin + new Vector3(0.0f, VoxelWorld.BlockSize, 0.0f),
                                Vector3.ForwardLH * VoxelWorld.BlockSize, Vector3.Right * VoxelWorld.BlockSize, true, false, vertices, uvs, indices, normals);

                            var ao00 = VoxelMeshHelper.CalculateAmbient(tl, tb, tbl);
                            var ao01 = VoxelMeshHelper.CalculateAmbient(tl, tf, tfl);
                            var ao11 = VoxelMeshHelper.CalculateAmbient(tr, tf, tfr);
                            var ao10 = VoxelMeshHelper.CalculateAmbient(tb, tr, tbr);

                            colors.Add(new Color(ao00 * 0.44f, 0.0f, 0.0f, 0.0f)); // bl
                            colors.Add(new Color(ao01 * 0.44f, 0.0f, 0.0f, 0.0f)); // ul
                            colors.Add(new Color(ao11 * 0.44f, 0.0f, 0.0f, 0.0f)); // ur
                            colors.Add(new Color(ao10 * 0.44f, 0.0f, 0.0f, 0.0f)); // br
                        }

                        // back face
                        if (!BlockExists(x, y, z - 1))
                        {
                            VoxelMeshHelper.SetupFace(
                                origin + new Vector3(0.0f, 0.0f, 0.0f),
                                Vector3.Up * VoxelWorld.BlockSize, Vector3.Right * VoxelWorld.BlockSize, true, false, vertices, uvs, indices, normals);

                            var ao00 = VoxelMeshHelper.CalculateAmbient(bb, mbl, bbl);
                            var ao01 = VoxelMeshHelper.CalculateAmbient(mbl, tb, tbl);
                            var ao11 = VoxelMeshHelper.CalculateAmbient(mbr, tb, tbr);
                            var ao10 = VoxelMeshHelper.CalculateAmbient(mbr, bb, bbr);

                            colors.Add(new Color(ao00 * 0.44f, 0.0f, 0.0f, 0.0f)); // ll
                            colors.Add(new Color(ao01 * 0.44f, 0.0f, 0.0f, 0.0f)); // ul
                            colors.Add(new Color(ao11 * 0.44f, 0.0f, 0.0f, 0.0f)); // ur
                            colors.Add(new Color(ao10 * 0.44f, 0.0f, 0.0f, 0.0f)); // lr
                        }

                        // front face
                        if (!BlockExists(x, y, z + 1))
                        {
                            VoxelMeshHelper.SetupFace(
                                origin + new Vector3(0.0f, 0.0f, VoxelWorld.BlockSize),
                                Vector3.Up * VoxelWorld.BlockSize, Vector3.Right * VoxelWorld.BlockSize, false, false, vertices, uvs, indices, normals);

                            colors.Add(new Color(0.0f, 0.0f, 0.0f, 0.0f));
                            colors.Add(new Color(0.0f, 0.0f, 0.0f, 0.0f));
                            colors.Add(new Color(0.0f, 0.0f, 0.0f, 0.0f));
                            colors.Add(new Color(0.0f, 0.0f, 0.0f, 0.0f));
                        }
                    }
                }
            }

            foreach (var vertices in blocksVertices)
            {
                var mesh = Mesh.Create();

                var verts = vertices.Value.ToArray();
                var indices = blocksIndices[vertices.Key].ToArray();
                var uvs = blocksUvs[vertices.Key].ToArray();
                var normals = blocksNormals[vertices.Key].ToArray();
                var colors = blocksColors[vertices.Key].ToArray();

                mesh.SetVertices(verts);
                mesh.SetIndices(indices);
                mesh.SetUVs(uvs);
                mesh.SetNormals(normals);
                mesh.SetColors(colors);

                if (verts.Length > 0)
                {
                    mesh.ApplyChanges();
                }

                Meshes.Add(vertices.Key, mesh);
            }

            IsVisible = true;
        }

        /// <summary>
        /// Update the chunk's lighting.
        /// </summary>
        public void UpdateLighting()
        {
            
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

        /// <summary>
        /// Check if local coordinate is on chunk edge.
        /// </summary>
        /// <param name="localCoord">The local coord.</param>
        /// <returns>True when coord in on edge.</returns>
        public static bool IsOnEdge(Int3 localCoord)
        {
            return localCoord.X == 0 || 
                   localCoord.Y == 0 || 
                   localCoord.Z == 0 ||
                   localCoord.X == VoxelWorld.ChunkSize - 1    || 
                   localCoord.Y == VoxelWorld.ChunkHeight - 1  || 
                   localCoord.Z == VoxelWorld.ChunkSize - 1;
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
            if (localCoord.X == 0 && localCoord.Z == VoxelWorld.ChunkSize-1)
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
            if (localCoord.X != VoxelWorld.ChunkSize - 1 && localCoord.Z ==  0)
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
    }
}