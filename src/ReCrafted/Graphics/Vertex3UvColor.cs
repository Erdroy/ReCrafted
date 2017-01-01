// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

using SharpDX;

namespace ReCrafted.Graphics
{
    /// <summary>
    /// Vertex3 structure.
    /// </summary>
    public struct Vertex3UvColor
    {
        /// <summary>
        /// Vertice
        /// </summary>
        public Vector3 Vertice;

        /// <summary>
        /// Uv
        /// </summary>
        public Vector2 Uv;

        /// <summary>
        /// Color
        /// </summary>
        public Vector4 Color;

        /// <summary>
        /// Size in bytes.
        /// </summary>
        public static readonly int SizeInBytes = Vector3.SizeInBytes + Vector2.SizeInBytes + Vector4.SizeInBytes;
    }
}
