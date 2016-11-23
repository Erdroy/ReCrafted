using System;
using ReCrafted.Utilities;
using SharpDX.Direct3D11;
using Buffer = SharpDX.Direct3D11.Buffer;

namespace ReCrafted.Graphics.Renderers.D3D11
{
    /// <summary>
    /// D3D11Mesh class.
    /// </summary>
    public sealed class D3D11Mesh : Mesh
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

            if (HasUVs && HasColors)
            {
                if(UVs.Length != Vertices.Length)
                    throw new ReCraftedException($"There is invalid amount of UVs! UV count {UVs.Length} Vertice count {Vertices.Length}, they must match!");

                if (Colors.Length != Vertices.Length)
                    throw new ReCraftedException($"There is invalid amount of Colors! Color count {Colors.Length} Vertice count {Vertices.Length}, they must match!");

                var vertexes = new Vertex3[Vertices.Length];

                // build data
                for (var i = 0; i < vertexes.Length; i++)
                {
                    vertexes[i] = new Vertex3
                    {
                        Vertice = Vertices[i],
                        Uv = UVs[i],
                        Color = Colors[i]
                    };
                }
                
                VertexBuffer = Buffer.Create(D3D11Renderer.GetDevice(), BindFlags.VertexBuffer, vertexes, vertexes.Length * Vertex3.SizeInBytes);

            }
            else if (HasUVs)
            {
                if (UVs.Length != Vertices.Length)
                    throw new ReCraftedException($"There is invalid amount of UVs! UV count {UVs.Length} Vertice count {Vertices.Length}, they must match!");

                var vertexes = new Vertex2[Vertices.Length];

                // build data
                for (var i = 0; i < vertexes.Length; i++)
                {
                    vertexes[i] = new Vertex2
                    {
                        Vertice = Vertices[i],
                        Uv = UVs[i]
                    };
                }

                VertexBuffer = Buffer.Create(D3D11Renderer.GetDevice(), BindFlags.VertexBuffer, vertexes, vertexes.Length * Vertex2.SizeInBytes);
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
        /// Gets the vertex stride.
        /// </summary>
        /// <returns>The vertex stride.</returns>
        public override int GetVertexStride()
        {
            if (HasUVs && HasColors)
            {
                return Vertex3.SizeInBytes;
            }

            return HasUVs ? Vertex2.SizeInBytes : Vertex.SizeInBytes;
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