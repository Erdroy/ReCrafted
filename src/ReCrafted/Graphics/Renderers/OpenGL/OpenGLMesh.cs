// ReCrafted © 2016 Damian 'Erdroy' Korczowski

using System.Diagnostics.CodeAnalysis;
using SharpDX;

namespace ReCrafted.Graphics.Renderers.OpenGL
{
    [SuppressMessage("ReSharper", "InconsistentNaming")]
    internal class OpenGLMesh : Mesh
    {
        public override void SetColors(Color[] colors)
        {
            throw new System.NotImplementedException();
        }

        public override void SetVertices(Vector3[] vertices)
        {
            throw new System.NotImplementedException();
        }

        public override void SetUVs(Vector2[] uvs)
        {
            throw new System.NotImplementedException();
        }

        public override void SetNormals(Vector3[] normals)
        {
            throw new System.NotImplementedException();
        }

        public override int GetVertexStride()
        {
            throw new System.NotImplementedException();
        }

        public override void ApplyChanges()
        {
            throw new System.NotImplementedException();
        }

        public override void Optimize()
        {
            throw new System.NotImplementedException();
        }

        public override void Dispose()
        {
            throw new System.NotImplementedException();
        }
    }
}
