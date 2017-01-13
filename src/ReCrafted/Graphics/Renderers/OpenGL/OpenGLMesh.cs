// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

using System;
using OpenTK.Graphics.OpenGL;
using ReCrafted.Utilities;
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
            if(VertexArrayObject >= 0)
                Dispose();

            // update buffers
            if (!HasVertices)
                throw new ReCraftedException("Cannot apply changes - there is no any vertices!");

            // source: http://stackoverflow.com/questions/27183668/binding-opengl-vertex-buffers-per-frame-for-multiple-meshes

            VertexArrayObject = GL.GenVertexArray();
            VertexBufferObject = GL.GenBuffer();

            GL.BindVertexArray(VertexArrayObject);
            GL.BindBuffer(BufferTarget.ArrayBuffer, VertexBufferObject);

            /* 
             * GL.BufferData(BufferTarget.ArrayBuffer, (IntPtr)(triangles.Length * 96), data, BufferUsageHint.StaticDraw);
             * GL.VertexAttribPointer(0, 3,  VertexAttribPointerType.Float, 0, 0);
             * GL.VertexAttribPointer(1, 3,  VertexAttribPointerType.Float, 0, triangles.Length * 9*4);
             * GL.VertexAttribPointer(2, 3,  VertexAttribPointerType.Float, 0, triangles.Length * 18*4);
             */

            if (HasUVs && HasColors && HasNormals)
            {
                if (UVs.Length != Vertices.Length)
                    throw new ReCraftedException($"There is invalid amount of UVs! UV count {UVs.Length} Vertice count {Vertices.Length}, they must match!");

                if (Colors.Length != Vertices.Length)
                    throw new ReCraftedException($"There is invalid amount of Colors! Color count {Colors.Length} Vertice count {Vertices.Length}, they must match!");

                if (Normals.Length != Vertices.Length)
                    throw new ReCraftedException($"There is invalid amount of Normals! Normals count {Normals.Length} Vertice count {Vertices.Length}, they must match!");

                var vertexes = new Vertex4UvNormalColor[Vertices.Length];

                // build data
                for (var i = 0; i < vertexes.Length; i++)
                {
                    vertexes[i] = new Vertex4UvNormalColor
                    {
                        Vertice = Vertices[i],
                        Uv = UVs[i],
                        Color = Colors[i].ToVector4(),
                        Normal = Normals[i]
                    };
                }

                // create buffer
                GL.BufferData(BufferTarget.ArrayBuffer, (IntPtr)(vertexes.Length * Vertex4UvNormalColor.SizeInBytes), vertexes, BufferUsageHint.StaticDraw);

                GL.EnableVertexAttribArray(0);
                GL.VertexAttribPointer(0, 3, VertexAttribPointerType.Float, false, Vector3.SizeInBytes, 0); // vert
                GL.EnableVertexAttribArray(1);
                GL.VertexAttribPointer(1, 2, VertexAttribPointerType.Float, false, Vector2.SizeInBytes, Vector3.SizeInBytes); // uv
                GL.EnableVertexAttribArray(2);
                GL.VertexAttribPointer(2, 3, VertexAttribPointerType.Float, false, Vector3.SizeInBytes, Vector3.SizeInBytes + Vector2.SizeInBytes); // norm
                GL.EnableVertexAttribArray(3);
                GL.VertexAttribPointer(3, 4, VertexAttribPointerType.Float, false, Vector4.SizeInBytes, Vector3.SizeInBytes + Vector2.SizeInBytes + Vector3.SizeInBytes); // col

            }
            else if (HasUVs && HasNormals)
            {
                if (UVs.Length != Vertices.Length)
                    throw new ReCraftedException($"There is invalid amount of UVs! UV count {UVs.Length} Vertice count {Vertices.Length}, they must match!");

                if (Normals.Length != Vertices.Length)
                    throw new ReCraftedException($"There is invalid amount of Normals! Normals count {Normals.Length} Vertice count {Vertices.Length}, they must match!");

                var vertexes = new Vertex3UvNormal[Vertices.Length];

                // build data
                for (var i = 0; i < vertexes.Length; i++)
                {
                    vertexes[i] = new Vertex3UvNormal
                    {
                        Vertice = Vertices[i],
                        Uv = UVs[i],
                        Normal = Normals[i]
                    };
                }

                // create buffer
                GL.BufferData(BufferTarget.ArrayBuffer, (IntPtr)(vertexes.Length * Vertex3UvNormal.SizeInBytes), vertexes, BufferUsageHint.StaticDraw);

                // create buffer
                GL.EnableVertexAttribArray(0);
                GL.VertexAttribPointer(0, 3, VertexAttribPointerType.Float, false, Vector3.SizeInBytes, 0); // vert
                GL.EnableVertexAttribArray(1);
                GL.VertexAttribPointer(1, 2, VertexAttribPointerType.Float, false, Vector2.SizeInBytes, Vector3.SizeInBytes); // uv
                GL.EnableVertexAttribArray(2);
                GL.VertexAttribPointer(2, 3, VertexAttribPointerType.Float, false, Vector3.SizeInBytes, Vector3.SizeInBytes + Vector2.SizeInBytes); // norm
            }
            else if (HasUVs && HasColors)
            {
                if (UVs.Length != Vertices.Length)
                    throw new ReCraftedException($"There is invalid amount of UVs! UV count {UVs.Length} Vertice count {Vertices.Length}, they must match!");

                if (Colors.Length != Vertices.Length)
                    throw new ReCraftedException($"There is invalid amount of Colors! Color count {Colors.Length} Vertice count {Vertices.Length}, they must match!");

                var vertexes = new Vertex3UvColor[Vertices.Length];

                // build data
                for (var i = 0; i < vertexes.Length; i++)
                {
                    vertexes[i] = new Vertex3UvColor
                    {
                        Vertice = Vertices[i],
                        Uv = UVs[i],
                        Color = Colors[i].ToVector4()
                    };
                }

                // create buffer
                GL.BufferData(BufferTarget.ArrayBuffer, (IntPtr)(vertexes.Length * Vertex3UvColor.SizeInBytes), vertexes, BufferUsageHint.StaticDraw);

                // create buffer
                GL.EnableVertexAttribArray(0);
                GL.VertexAttribPointer(0, 3, VertexAttribPointerType.Float, false, Vector3.SizeInBytes, 0); // vert
                GL.EnableVertexAttribArray(1);
                GL.VertexAttribPointer(1, 2, VertexAttribPointerType.Float, false, Vector2.SizeInBytes, Vector3.SizeInBytes); // uv
                GL.EnableVertexAttribArray(2);
                GL.VertexAttribPointer(2, 4, VertexAttribPointerType.Float, false, Vector4.SizeInBytes, Vector3.SizeInBytes + Vector2.SizeInBytes); // col
            }
            else if (HasColors)
            {
                var vertexes = new Vertex2Color[Vertices.Length];

                // build data
                for (var i = 0; i < vertexes.Length; i++)
                {
                    vertexes[i] = new Vertex2Color
                    {
                        Vertice = Vertices[i],
                        Color = Colors[i].ToVector4()
                    };
                }

                // create buffer
                GL.BufferData(BufferTarget.ArrayBuffer, (IntPtr)(vertexes.Length * Vertex2Color.SizeInBytes), vertexes, BufferUsageHint.StaticDraw);

                // create buffer
                GL.EnableVertexAttribArray(0);
                GL.VertexAttribPointer(0, 3, VertexAttribPointerType.Float, false, Vector3.SizeInBytes, 0); // vert
                GL.EnableVertexAttribArray(1);
                GL.VertexAttribPointer(1, 4, VertexAttribPointerType.Float, false, Vector4.SizeInBytes, Vector3.SizeInBytes); // col
            }
            else if (HasUVs)
            {
                if (UVs.Length != Vertices.Length)
                    throw new ReCraftedException($"There is invalid amount of UVs! UV count {UVs.Length} Vertice count {Vertices.Length}, they must match!");

                var vertexes = new Vertex2Uv[Vertices.Length];

                // build data
                for (var i = 0; i < vertexes.Length; i++)
                {
                    vertexes[i] = new Vertex2Uv
                    {
                        Vertice = Vertices[i],
                        Uv = UVs[i]
                    };
                }

                // create buffer
                GL.BufferData(BufferTarget.ArrayBuffer, (IntPtr)(vertexes.Length * Vertex2Uv.SizeInBytes), vertexes, BufferUsageHint.StaticDraw);

                // create buffer
                GL.EnableVertexAttribArray(0);
                GL.VertexAttribPointer(0, 3, VertexAttribPointerType.Float, false, Vector3.SizeInBytes, 0); // vert
                GL.EnableVertexAttribArray(1);
                GL.VertexAttribPointer(1, 2, VertexAttribPointerType.Float, false, Vector2.SizeInBytes, Vector3.SizeInBytes); // uv
            }
            else
            {
                var vertexes = new Vertex[Vertices.Length];

                // build data
                for (var i = 0; i < vertexes.Length; i++)
                {
                    vertexes[i] = new Vertex
                    {
                        Vertice = Vertices[i]
                    };
                }

                // create buffer
                GL.BufferData(BufferTarget.ArrayBuffer, (IntPtr)(vertexes.Length * Vertex.SizeInBytes), vertexes, BufferUsageHint.StaticDraw);

                // create buffer
                GL.EnableVertexAttribArray(0);
                GL.VertexAttribPointer(0, 3, VertexAttribPointerType.Float, true, Vector3.SizeInBytes, 0); // vert
            }
            
            if (HasIndices)
            {
                // build indices buffer
                ElementBufferObject = GL.GenBuffer();
                GL.BindBuffer(BufferTarget.ElementArrayBuffer, ElementBufferObject);
                GL.BufferData(BufferTarget.ElementArrayBuffer, (IntPtr)(Indices.Length * sizeof(uint)), Indices, BufferUsageHint.StaticDraw);
            }

            GL.BindBuffer(BufferTarget.ArrayBuffer, 0);
            GL.BindBuffer(BufferTarget.ElementArrayBuffer, 0);
            GL.BindVertexArray(0);
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
            if(VertexArrayObject >= 0)
                GL.DeleteVertexArray(VertexArrayObject);

            if(VertexBufferObject >= 0)
                GL.DeleteBuffer(VertexBufferObject);

            if(ElementBufferObject >= 0)
                GL.DeleteBuffer(ElementBufferObject);

            VertexArrayObject = -1;
            VertexBufferObject = -1;
            ElementBufferObject = -1;
        }

        public int VertexArrayObject { get; private set; } = -1;
        public int VertexBufferObject { get; private set; } = -1;
        public int ElementBufferObject { get; private set; } = -1;
    }
}