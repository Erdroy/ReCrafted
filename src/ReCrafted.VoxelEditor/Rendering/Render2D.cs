// ReCrafted © 2016-2017 Always Too Late

using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using SharpDX;
using SharpDX.Direct3D11;
using SharpDX.DXGI;
using Buffer = SharpDX.Direct3D11.Buffer;

namespace ReCrafted.VoxelEditor.Rendering
{
    public class Render2D : IDisposable
    {
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        public struct Vertex
        {
            public Vector3 Position;
            public Vector2 TexCoord;
            public Color4 Color;

            public static readonly int SizeInBytes = Vector3.SizeInBytes + Vector2.SizeInBytes + 4 * sizeof(float);
        }

        public struct DrawCmd
        {
            public Vertex[] Vertices { get; set; }
            //public Texture2D Texture { get; set; }
            public int ZOrder { get; set; }
        }

        private const int MaxVertices = 65535;

        private readonly List<DrawCmd> _drawCommands = new List<DrawCmd>();
        private readonly List<Vertex> _vertices = new List<Vertex>();
        private readonly List<ushort> _indices = new List<ushort>();
        
        public void Init()
        {
            
        }

        public void Add(DrawCmd drawCmd)
        {
            _drawCommands.Add(drawCmd);
        }

        public void Draw()
        {
            // sort by ZOrder
            _drawCommands.Sort((cmdA, cmdB) =>
            {
                if (cmdA.ZOrder > cmdB.ZOrder)
                    return -1;

                return cmdA.ZOrder < cmdB.ZOrder ? 1 : 0;
            });

            foreach (var drawCmd in _drawCommands)
            {
                // if buffers are full, or texture changes, draw
                if (_vertices.Count + 4 > MaxVertices)
                {
                    DrawNow();
                }

                // submit
                PushDrawCmd(drawCmd);
            }

            // if something is still in the buffers, draw
            if (_vertices.Count > 0)
            {
                DrawNow();
            }

            _drawCommands.Clear();
        }

        public void Dispose()
        {
            _drawCommands.Clear();
        }

        private void DrawNow()
        {
            // create buffers and upload data
            var vertexBuffer = Buffer.Create(Renderer.Current.D3D11Device, BindFlags.VertexBuffer, _vertices.ToArray());
            var indexBuffer = Buffer.Create(Renderer.Current.D3D11Device, BindFlags.IndexBuffer, _indices.ToArray());

            // draw buffers
            Renderer.Current.DeviceContext.InputAssembler.SetVertexBuffers(0, new VertexBufferBinding(vertexBuffer, Vertex.SizeInBytes, 0));
            Renderer.Current.DeviceContext.InputAssembler.SetIndexBuffer(indexBuffer, Format.R16_UInt, 0);

            // draw
            Renderer.Current.DeviceContext.DrawIndexed(_indices.Count, 0, 0);

            // cleanup
            vertexBuffer.Dispose();
            indexBuffer.Dispose();
            _vertices.Clear();
            _indices.Clear();

            // TODO: buffer reuse?
        }

        private void PushDrawCmd(DrawCmd cmd)
        {
            var index = _vertices.Count;
            
            // add all vertices
            _vertices.AddRange(cmd.Vertices);

            // add all indices
            _indices.Add((ushort)(index + 0));
            _indices.Add((ushort)(index + 1));
            _indices.Add((ushort)(index + 2));

            _indices.Add((ushort)(index + 2));
            _indices.Add((ushort)(index + 3));
            _indices.Add((ushort)(index + 0));
        }
    }
}
