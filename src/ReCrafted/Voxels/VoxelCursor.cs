// ReCrafted © 2016 Damian 'Erdroy' Korczowski

using System;
using ReCrafted.Graphics;
using SharpDX;

namespace ReCrafted.Voxels
{
    /// <summary>
    /// VoxelCursor class.
    /// </summary>
    public sealed class VoxelCursor : IDisposable
    {
        /// <summary>
        /// The latest instance of voxel cursor.
        /// </summary>
        public static VoxelCursor Instance;

        private Mesh _mesh;
        private Shader _shader;

        /// <summary>
        /// VoxelCursor constructor.
        /// </summary>
        public VoxelCursor()
        {
            Instance = this;

            // initialize default values
            Enabled = true;
            MaximumDistance = 7;
            CursorPosition = new Vector3(0.0f);
        }

        /// <summary>
        /// Initialize the voxel cursor.
        /// </summary>
        public void Init()
        {
            _mesh = Mesh.Create();
            _mesh.SetVertices(new []
            {
                new Vector3(-0.01f, -0.01f, -0.01f),
                new Vector3(-0.01f, 1.010f, -0.01f),

                new Vector3(1.010f, -0.01f, 1.010f),
                new Vector3(1.010f, 1.010f, 1.010f),

                new Vector3(1.010f, -0.01f, -0.01f),
                new Vector3(1.010f, 1.010f, -0.01f),

                new Vector3(-0.01f, -0.01f, 1.010f),
                new Vector3(-0.01f, 1.010f, 1.010f),
                
                new Vector3(-0.01f, -0.01f, -0.01f),
                new Vector3(-0.01f, -0.01f, 1.010f),

                new Vector3(-0.01f, -0.01f, -0.01f),
                new Vector3(1.010f, -0.01f, -0.01f),
                
                new Vector3(-0.01f, 1.010f, -0.01f),
                new Vector3(-0.01f, 1.010f, 1.010f),

                new Vector3(-0.01f, 1.010f, -0.01f),
                new Vector3(1.010f, 1.010f, -0.01f),
                
                new Vector3(1.010f, -0.01f, 1.010f),
                new Vector3(-0.01f, -0.01f, 1.010f),

                new Vector3(1.010f, -0.01f, 1.010f),
                new Vector3(1.010f, -0.01f, -0.01f),

                new Vector3(1.010f, 1.010f, 1.010f),
                new Vector3(-0.01f, 1.010f, 1.010f),

                new Vector3(1.010f, 1.010f, 1.010f),
                new Vector3(1.010f, 1.010f, -0.01f),
            });

            // apply mesh changes
            _mesh.ApplyChanges();

            // set the primitive type as line list
            _mesh.PrimitiveType = PrimitiveType.LineList;

            _shader = Shader.FromFile("cursor");
        }

        /// <summary>
        /// Tick the voxel cursor.
        /// </summary>
        public void Tick()
        {
            ushort blockId;
            Int3 blockCoords;
            VoxelChunk chunk;
            Vector3 normal;
            VoxelWorld.Instance.RaycastBlocks(Camera.Current.Position, Camera.Current.Forward, MaximumDistance, out blockId, out blockCoords, out chunk, out normal);

            VoxelChunk = chunk;
            Normal = normal;

            if (chunk == null) // null when not hit any block
                return;

            // we got hit!
            var worldCoord = chunk.Position + blockCoords;
            BlockCoords = blockCoords;
            CursorPosition = (Vector3)worldCoord;
        }
        
        /// <summary>
        /// Draw the voxel cursor.
        /// </summary>
        public void Draw()
        {
            if (!Enabled)
                return;

            if (VoxelChunk == null)
                return;

            var wvp = Matrix.Translation(CursorPosition) * Camera.Current.ViewProjectionMatrix;
            wvp.Transpose();

            _shader.SetValue("WVP", wvp);
            _shader.Draw(_mesh);
        }

        /// <summary>
        /// Dispose the voxel cursor.
        /// </summary>
        public void Dispose()
        {
            _mesh?.Dispose();
        }

        /// <summary>
        /// The position of cursor.
        /// </summary>
        public Vector3 CursorPosition { get; set; }

        /// <summary>
        /// The normal of hit from raycast.
        /// </summary>
        public Vector3 Normal { get; set; }

        /// <summary>
        /// The hit block coords.
        /// </summary>
        public Int3 BlockCoords { get; set; }

        /// <summary>
        /// The hit voxel chunk.
        /// </summary>
        public VoxelChunk VoxelChunk { get; set; }

        /// <summary>
        /// The maximum distance for block selecting(in blocks).
        /// Default: 7.
        /// </summary>
        public int MaximumDistance { get; set; }

        /// <summary>
        /// Is the VoxelCursor enabled?
        /// </summary>
        public bool Enabled { get; set; }
    }
}