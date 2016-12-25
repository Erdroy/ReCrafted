// ReCrafted © 2016 Damian 'Erdroy' Korczowski

using System;
using ReCrafted.Utilities;
using SharpDX;
using SharpDX.Direct3D11;
using Buffer = SharpDX.Direct3D11.Buffer;

namespace ReCrafted.Graphics.Renderers.D3D11
{
    /// <summary>
    /// D3D11Mesh class.
    /// </summary>
    internal sealed class D3D11Mesh : Mesh
    {

        internal D3D11Mesh() { }

        /// <summary>
        /// Applies changes.
        /// </summary>
        public override void ApplyChanges()
        {
            VertexBuffer?.Dispose();
            IndexBuffer?.Dispose();
            
            // update buffers
            if (!HasVertices)
                throw new ReCraftedException("Cannot apply changes - there is no any vertices!");

            if (HasUVs && HasNormals)
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

                VertexBuffer = Buffer.Create(D3D11Renderer.GetDevice(), BindFlags.VertexBuffer, vertexes, vertexes.Length * Vertex3UvNormal.SizeInBytes);
            }
            else if (HasUVs && HasColors)
            {
                if(UVs.Length != Vertices.Length)
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
                
                VertexBuffer = Buffer.Create(D3D11Renderer.GetDevice(), BindFlags.VertexBuffer, vertexes, vertexes.Length * Vertex3UvColor.SizeInBytes);

            }
            else if (HasUVs && HasColors && HasNormals)
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

                VertexBuffer = Buffer.Create(D3D11Renderer.GetDevice(), BindFlags.VertexBuffer, vertexes, vertexes.Length * Vertex4UvNormalColor.SizeInBytes);

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

                VertexBuffer = Buffer.Create(D3D11Renderer.GetDevice(), BindFlags.VertexBuffer, vertexes, vertexes.Length * Vertex2Color.SizeInBytes);
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

                VertexBuffer = Buffer.Create(D3D11Renderer.GetDevice(), BindFlags.VertexBuffer, vertexes, vertexes.Length * Vertex2Uv.SizeInBytes);
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

                VertexBuffer = Buffer.Create(D3D11Renderer.GetDevice(), BindFlags.VertexBuffer, vertexes, vertexes.Length * Vertex.SizeInBytes);
            }

            if (HasIndices)
            {
                // build indices buffer
                IndexBuffer = Buffer.Create(D3D11Renderer.GetDevice(), BindFlags.IndexBuffer, Indices, Indices.Length * sizeof(uint));
            }

            VertexBufferBinding = new VertexBufferBinding(VertexBuffer, GetVertexStride(), 0);
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
                stride += 4;

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
            // dispose buffers
            VertexBuffer?.Dispose();
            IndexBuffer?.Dispose();
        }
        
        /// <summary>
        /// Vertex buffer binding.
        /// </summary>
        internal VertexBufferBinding VertexBufferBinding { get; private set; }

        /// <summary>
        /// Vertex buffer.
        /// </summary>
        internal Buffer VertexBuffer { get; private set; }

        /// <summary>
        /// Index buffer.
        /// </summary>
        internal Buffer IndexBuffer { get; private set; }
    }
}