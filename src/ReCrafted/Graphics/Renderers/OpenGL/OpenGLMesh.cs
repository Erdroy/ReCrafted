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
        private int[] _buffers;

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

            GL.BindVertexArray(VertexArrayObject);
            
            if (HasUVs && HasColors && HasNormals)
            {
                if (UVs.Length != Vertices.Length)
                    throw new ReCraftedException($"There is invalid amount of UVs! UV count {UVs.Length} Vertice count {Vertices.Length}, they must match!");

                if (Colors.Length != Vertices.Length)
                    throw new ReCraftedException($"There is invalid amount of Colors! Color count {Colors.Length} Vertice count {Vertices.Length}, they must match!");

                if (Normals.Length != Vertices.Length)
                    throw new ReCraftedException($"There is invalid amount of Normals! Normals count {Normals.Length} Vertice count {Vertices.Length}, they must match!");

                _buffers = new int[4];
                GL.GenBuffers(4, _buffers);

                // VERTICE
                GL.EnableVertexAttribArray(0);
                GL.BindBuffer(BufferTarget.ArrayBuffer, _buffers[0]);
                GL.BufferData(BufferTarget.ArrayBuffer, (IntPtr)(Vertices.Length * Vector3.SizeInBytes), Vertices, BufferUsageHint.StaticDraw);
                GL.VertexAttribPointer(0, 3, VertexAttribPointerType.Float, false, Vector3.SizeInBytes, 0);

                // UV
                GL.EnableVertexAttribArray(1);
                GL.BindBuffer(BufferTarget.ArrayBuffer, _buffers[1]);
                GL.BufferData(BufferTarget.ArrayBuffer, (IntPtr)(UVs.Length * Vector2.SizeInBytes), UVs, BufferUsageHint.StaticDraw);
                GL.VertexAttribPointer(1, 2, VertexAttribPointerType.Float, false, Vector2.SizeInBytes, 0);

                // NORMAL
                GL.EnableVertexAttribArray(2);
                GL.BindBuffer(BufferTarget.ArrayBuffer, _buffers[2]);
                GL.BufferData(BufferTarget.ArrayBuffer, (IntPtr)(Normals.Length * Vector3.SizeInBytes), Normals, BufferUsageHint.StaticDraw);
                GL.VertexAttribPointer(2, 3, VertexAttribPointerType.Float, false, Vector3.SizeInBytes, 0);

                // COLOR
                GL.EnableVertexAttribArray(3);
                GL.BindBuffer(BufferTarget.ArrayBuffer, _buffers[3]);
                GL.BufferData(BufferTarget.ArrayBuffer, (IntPtr)(Colors.Length * Vector4.SizeInBytes), Colors, BufferUsageHint.StaticDraw);
                GL.VertexAttribPointer(3, 4, VertexAttribPointerType.Float, false, Vector4.SizeInBytes, 0);


            }
            else if (HasUVs && HasNormals)
            {
                if (UVs.Length != Vertices.Length)
                    throw new ReCraftedException($"There is invalid amount of UVs! UV count {UVs.Length} Vertice count {Vertices.Length}, they must match!");

                if (Normals.Length != Vertices.Length)
                    throw new ReCraftedException($"There is invalid amount of Normals! Normals count {Normals.Length} Vertice count {Vertices.Length}, they must match!");

                _buffers = new int[3];
                GL.GenBuffers(3, _buffers);

                // VERTICE
                GL.EnableVertexAttribArray(0);
                GL.BindBuffer(BufferTarget.ArrayBuffer, _buffers[0]);
                GL.BufferData(BufferTarget.ArrayBuffer, (IntPtr)(Vertices.Length * Vector3.SizeInBytes), Vertices, BufferUsageHint.StaticDraw);
                GL.VertexAttribPointer(0, 3, VertexAttribPointerType.Float, false, Vector3.SizeInBytes, 0);

                // UV
                GL.EnableVertexAttribArray(1);
                GL.BindBuffer(BufferTarget.ArrayBuffer, _buffers[1]);
                GL.BufferData(BufferTarget.ArrayBuffer, (IntPtr)(UVs.Length * Vector2.SizeInBytes), UVs, BufferUsageHint.StaticDraw);
                GL.VertexAttribPointer(1, 2, VertexAttribPointerType.Float, false, Vector2.SizeInBytes, 0);

                // NORMAL
                GL.EnableVertexAttribArray(2);
                GL.BindBuffer(BufferTarget.ArrayBuffer, _buffers[2]);
                GL.BufferData(BufferTarget.ArrayBuffer, (IntPtr)(Normals.Length * Vector3.SizeInBytes), Normals, BufferUsageHint.StaticDraw);
                GL.VertexAttribPointer(2, 3, VertexAttribPointerType.Float, false, Vector3.SizeInBytes, 0);
            }
            else if (HasUVs && HasColors)
            {
                if (UVs.Length != Vertices.Length)
                    throw new ReCraftedException($"There is invalid amount of UVs! UV count {UVs.Length} Vertice count {Vertices.Length}, they must match!");

                if (Colors.Length != Vertices.Length)
                    throw new ReCraftedException($"There is invalid amount of Colors! Color count {Colors.Length} Vertice count {Vertices.Length}, they must match!");

                _buffers = new int[3];
                GL.GenBuffers(3, _buffers);

                // VERTICE
                GL.EnableVertexAttribArray(0);
                GL.BindBuffer(BufferTarget.ArrayBuffer, _buffers[0]);
                GL.BufferData(BufferTarget.ArrayBuffer, (IntPtr)(Vertices.Length * Vector3.SizeInBytes), Vertices, BufferUsageHint.StaticDraw);
                GL.VertexAttribPointer(0, 3, VertexAttribPointerType.Float, false, Vector3.SizeInBytes, 0);

                // UV
                GL.EnableVertexAttribArray(1);
                GL.BindBuffer(BufferTarget.ArrayBuffer, _buffers[1]);
                GL.BufferData(BufferTarget.ArrayBuffer, (IntPtr)(UVs.Length * Vector2.SizeInBytes), UVs, BufferUsageHint.StaticDraw);
                GL.VertexAttribPointer(1, 2, VertexAttribPointerType.Float, false, Vector2.SizeInBytes, 0);

                // COLOR
                GL.EnableVertexAttribArray(2);
                GL.BindBuffer(BufferTarget.ArrayBuffer, _buffers[2]);
                GL.BufferData(BufferTarget.ArrayBuffer, (IntPtr)(Colors.Length * Vector4.SizeInBytes), Colors, BufferUsageHint.StaticDraw);
                GL.VertexAttribPointer(2, 4, VertexAttribPointerType.Float, false, Vector4.SizeInBytes, 0);
            }
            else if (HasColors)
            {
                if (Colors.Length != Vertices.Length)
                    throw new ReCraftedException($"There is invalid amount of Colors! Colors count {Colors.Length} Vertice count {Vertices.Length}, they must match!");

                _buffers = new int[2];
                GL.GenBuffers(2, _buffers);

                // VERTICE
                GL.EnableVertexAttribArray(0);
                GL.BindBuffer(BufferTarget.ArrayBuffer, _buffers[0]);
                GL.BufferData(BufferTarget.ArrayBuffer, (IntPtr)(Vertices.Length * Vector3.SizeInBytes), Vertices, BufferUsageHint.StaticDraw);
                GL.VertexAttribPointer(0, 3, VertexAttribPointerType.Float, false, Vector3.SizeInBytes, 0);

                // COLOR
                GL.EnableVertexAttribArray(1);
                GL.BindBuffer(BufferTarget.ArrayBuffer, _buffers[1]);
                GL.BufferData(BufferTarget.ArrayBuffer, (IntPtr)(Colors.Length * Vector4.SizeInBytes), Colors, BufferUsageHint.StaticDraw);
                GL.VertexAttribPointer(1, 4, VertexAttribPointerType.Float, false, Vector4.SizeInBytes, 0);
            }
            else if (HasUVs)
            {
                if (UVs.Length != Vertices.Length)
                    throw new ReCraftedException($"There is invalid amount of UVs! UV count {UVs.Length} Vertice count {Vertices.Length}, they must match!");

                _buffers = new int[2];
                GL.GenBuffers(2, _buffers);

                // VERTICE
                GL.EnableVertexAttribArray(0);
                GL.BindBuffer(BufferTarget.ArrayBuffer, _buffers[0]);
                GL.BufferData(BufferTarget.ArrayBuffer, (IntPtr)(Vertices.Length * Vector3.SizeInBytes), Vertices, BufferUsageHint.StaticDraw);
                GL.VertexAttribPointer(0, 3, VertexAttribPointerType.Float, false, Vector3.SizeInBytes, 0);

                // UV
                GL.EnableVertexAttribArray(1);
                GL.BindBuffer(BufferTarget.ArrayBuffer, _buffers[1]);
                GL.BufferData(BufferTarget.ArrayBuffer, (IntPtr)(UVs.Length * Vector2.SizeInBytes), UVs, BufferUsageHint.StaticDraw);
                GL.VertexAttribPointer(1, 2, VertexAttribPointerType.Float, false, Vector2.SizeInBytes, 0);
            }
            else
            {
                _buffers = new int[1];
                GL.GenBuffers(1, _buffers);

                // VERTICE
                GL.EnableVertexAttribArray(0);
                GL.BindBuffer(BufferTarget.ArrayBuffer, _buffers[0]);
                GL.BufferData(BufferTarget.ArrayBuffer, (IntPtr)(Vertices.Length * Vector3.SizeInBytes), Vertices, BufferUsageHint.StaticDraw);
                GL.VertexAttribPointer(0, 3, VertexAttribPointerType.Float, false, Vector3.SizeInBytes, 0);
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
            
            if(ElementBufferObject >= 0)
                GL.DeleteBuffer(ElementBufferObject);

            VertexArrayObject = -1;
            ElementBufferObject = -1;
        }

        public int VertexArrayObject { get; private set; } = -1;
        public int ElementBufferObject { get; private set; } = -1;
    }
}