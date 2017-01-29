// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

using System;
using ReCrafted.Graphics.Renderers.D3D11;
using ReCrafted.Graphics.Renderers.OpenGL;
using ReCrafted.Utilities;
using SharpDX;

namespace ReCrafted.Graphics
{
    public abstract class Mesh : IDisposable
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
        public abstract void SetColors(Vector4[] colors);

        /// <summary>
        /// Sets the vertices array.
        /// </summary>
        /// <param name="vertices">The vertices.</param>
        public abstract void SetVertices(Vector3[] vertices);

        /// <summary>
        /// Set UVs array.
        /// </summary>
        /// <param name="uvs">The UVs array.</param>
        public abstract void SetUVs(Vector2[] uvs);

        /// <summary>
        /// Set Normals array.
        /// </summary>
        /// <param name="normals">The Normals array.</param>
        public abstract void SetNormals(Vector3[] normals);

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
        public abstract int GetVertexStride();

        /// <summary>
        /// Applies changes.
        /// </summary>
        public abstract void ApplyChanges();

        /// <summary>
        /// Optimizes the mesh.
        /// Should be called before apply changes!
        /// </summary>
        public abstract void Optimize();

        /// <summary>
        /// Disposes the mesh.
        /// </summary>
        public abstract void Dispose();
        
        /// <summary>
        /// Creates mesh from mesh data.
        /// </summary>
        /// <param name="meshData">The mesh data.</param>
        /// <returns>The created mesh.</returns>
        public static Mesh FromMeshData(MeshData meshData)
        {
            Mesh mesh;

            switch (Renderer.RendererApi)
            {
                case RendererApi.D3D11:
                    mesh = new D3D11Mesh();
                    break;
                case RendererApi.OpenGL:
                    mesh = new OpenGLMesh();
                    break;
                default:
                    return null;
                
                // TODO: Implement renderers
            }

            if (meshData.Positions == null || meshData.Positions.Length == 0)
            {
                throw new ReCraftedException("Cannot load mesh! Mesh does not have any positions defined!");
            }

            mesh.SetVertices(meshData.Positions);

            if (meshData.UVs != null && meshData.UVs.Length > 0)
                mesh.SetUVs(meshData.UVs);

            if (meshData.Colors != null && meshData.Colors.Length > 0)
                mesh.SetColors(meshData.Colors);

            if (meshData.Normals != null && meshData.Normals.Length > 0)
                mesh.SetNormals(meshData.Normals);

            if (meshData.Indices != null && meshData.Indices.Length > 0)
                mesh.SetIndices(meshData.Indices);

            mesh.ApplyChanges();

            return mesh;
        }

        /// <summary>
        /// Creates new mesh.
        /// </summary>
        /// <returns>The created mesh.</returns>
        public static Mesh Create()
        {
            switch (Renderer.RendererApi)
            {
                case RendererApi.D3D11:
                    return new D3D11Mesh();
                case RendererApi.OpenGL:
                    return new OpenGLMesh();
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
        /// Vertices amount.
        /// </summary>
        public int VerticeCount { get; protected set; }

        /// <summary>
        /// The colors array.
        /// </summary>
        public Vector4[] Colors { get; protected set; }

        /// <summary>
        /// The vertices array.
        /// </summary>
        public Vector3[] Vertices { get; protected set; }

        /// <summary>
        /// The UVs array.
        /// </summary>
        public Vector2[] UVs { get; protected set; }

        /// <summary>
        /// The Normals array.
        /// </summary>
        public Vector3[] Normals { get; protected set; }

        /// <summary>
        /// The index array.
        /// </summary>
        public uint[] Indices { get; protected set; }

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
        /// Do this mesh has normals?
        /// </summary>
        public bool HasNormals => Normals != null && Normals.Length > 0;

        /// <summary>
        /// Do this mesh has indices?
        /// </summary>
        public bool HasIndices => Indices != null && Indices.Length > 0;
    }
}