// ReCrafted (c) 2016-2018 Always Too Late

using System;
using System.Collections.Generic;
using ReCrafted.UI.Math;

namespace ReCrafted.UI
{
    public class UIBuffer : IDisposable
    {
        private readonly List<UIVertex> _vertices = new List<UIVertex>();
        private readonly List<uint> _indices = new List<uint>();

        public void Clear()
        {
            _vertices.Clear();
        }

        public void AddVertex(UIVertex vertex)
        {
            _vertices.Add(vertex);
        }

        public void AddVertexBuffer(UIBuffer buffer)
        {
            _vertices.AddRange(buffer.Vertices);
        }

        public void AddIndex(uint index)
        {
            _indices.Add(index);
        }

        public void Dispose()
        {
            Clear();
        }

        public void MakeRectangle(Position3D position, Size size, Color color)
        {
            // Add 4 vertices

        }

        public UIImage Image { get; set; }

        public IReadOnlyList<UIVertex> Vertices => _vertices;
        public IReadOnlyList<uint> Indices => _indices;
    }
}
