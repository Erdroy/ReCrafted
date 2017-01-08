// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

using System;
using SharpDX;

namespace ReCrafted.Graphics.Renderers.OpenGL
{
    /// <summary>
    /// OpenGLMesh class.
    /// </summary>
    // ReSharper disable once InconsistentNaming
    internal sealed class OpenGLMesh : Mesh
    {

        internal OpenGLMesh() { }

        /// <summary>
        /// Applies changes.
        /// </summary>
        public override void ApplyChanges()
        {
        }
        
        /// <summary>
        /// Sets colors array.
        /// </summary>
        /// <param name="colors">The colors array.</param>
        public override void SetColors(Color[] colors)
        {
            Colors = colors;
        }

        /// <summary>
        /// Sets the vertices array.
        /// </summary>
        /// <param name="vertices">The vertices.</param>
        public override void SetVertices(Vector3[] vertices)
        {
            Vertices = vertices;
        }

        /// <summary>
        /// Set UVs array.
        /// </summary>
        /// <param name="uvs">The UVs array.</param>
        public override void SetUVs(Vector2[] uvs)
        {
            UVs = uvs;
        }

        /// <summary>
        /// Set Normals array.
        /// </summary>
        /// <param name="normals">The Normals array.</param>
        public override void SetNormals(Vector3[] normals)
        {
            Normals = normals;
        }

        /// <summary>
        /// Gets the vertex stride.
        /// </summary>
        /// <returns>The vertex stride.</returns>
        public override int GetVertexStride()
        {
            var stride = Vector3.SizeInBytes;

            if (HasUVs)
                stride += Vector2.SizeInBytes;

            if (HasNormals)
                stride += Vector3.SizeInBytes;

            if (HasColors)
                stride += Vector4.SizeInBytes;

            return stride;
        }

        /// <summary>
        /// Optimizes the mesh.
        /// Should be called before apply changes!
        /// </summary>
        public override void Optimize()
        {
            throw new NotImplementedException("Not implemented");
        }

        /// <summary>
        /// Disposes the mesh.
        /// </summary>
        public override void Dispose()
        {
        }
    }
}