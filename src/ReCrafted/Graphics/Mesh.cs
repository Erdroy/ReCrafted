using System;
using ReCrafted.Graphics.Renderers.D3D11;
using ReCrafted.Utilities;
using SharpDX;

namespace ReCrafted.Graphics
{
    public class Mesh : IDisposable
    {
        // Constructor
        /// <summary>
        /// Mesh constructor.
        /// </summary>
        protected Mesh()
        {
            PrimitiveType = PrimitiveType.TriangleList;
        }

        // Methods
        /// <summary>
        /// Sets colors array.
        /// </summary>
        /// <param name="colors">The colors array.</param>
        public virtual void SetColors(Color[] colors)
        {
            Colors = colors;
        }

        /// <summary>
        /// Sets the vertices array.
        /// </summary>
        /// <param name="vertices">The vertices.</param>
        public virtual void SetVertices(Vector3[] vertices)
        {
            Vertices = vertices;
        }

        /// <summary>
        /// Set UVs array.
        /// </summary>
        /// <param name="uvs">The UVs array.</param>
        public virtual void SetUVs(Vector2[] uvs)
        {
            UVs = uvs;
        }

        /// <summary>
        /// Set index array.
        /// </summary>
        /// <param name="indices">The index array.</param>
        public virtual void SetIndices(uint[] indices)
        {
            Indices = indices;
        }

        /// <summary>
        /// Gets the vertex stride.
        /// </summary>
        /// <returns>The vertex stride.</returns>
        public virtual int GetVertexStride()
        {
            return 0;
        }

        /// <summary>
        /// Applies changes.
        /// </summary>
        public virtual void ApplyChanges() { }

        /// <summary>
        /// Optimizes the mesh.
        /// Should be called before apply changes!
        /// </summary>
        public virtual void Optimize() { }

        /// <summary>
        /// Disposes the mesh.
        /// </summary>
        public virtual void Dispose() { }

        /// <summary>
        /// Creates new mesh.
        /// </summary>
        /// <returns>The created mesh.</returns>
        public static Mesh Create()
        {
            if (Renderer.RendererApi == RendererApi.D3D11)
            {
                return new D3D11Mesh();
            }

            // TODO: Implement renderers

            throw new ReCraftedException("Invalid RendererApi!");
        }

        // Properties
        /// <summary>
        /// The primitive type.
        /// Default: triangle list.
        /// </summary>
        public PrimitiveType PrimitiveType { get; set; }

        /// <summary>
        /// The colors array.
        /// </summary>
        public Color[] Colors { get; private set; }

        /// <summary>
        /// The vertices array.
        /// </summary>
        public Vector3[] Vertices { get; private set; }

        /// <summary>
        /// The UVs array.
        /// </summary>
        public Vector2[] UVs { get; private set; }

        /// <summary>
        /// The index array.
        /// </summary>
        public uint[] Indices { get; private set; }

        /// <summary>
        /// Do this mesh has colors?
        /// </summary>
        public bool HasColors => Colors != null && Colors.Length > 0;

        /// <summary>
        /// Do this mesh has vertices?
        /// </summary>
        public bool HasVertices => Vertices != null && Vertices.Length > 0;

        /// <summary>
        /// Do this mesh has uvs?
        /// </summary>
        public bool HasUVs => UVs != null && UVs.Length > 0;

        /// <summary>
        /// Do this mesh has indices?
        /// </summary>
        public bool HasIndices => Indices != null && Indices.Length > 0;
    }
}