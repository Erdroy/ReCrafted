// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

using SharpDX;

namespace ReCrafted.Graphics
{
    /// <summary>
    /// Vertex4 structure.
    /// </summary>
    public struct Vertex4UvNormalColor
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
        /// Normal
        /// </summary>
        public Vector3 Normal;

        /// <summary>
        /// Color
        /// </summary>
        public Vector4 Color;

        /// <summary>
        /// Size in bytes.
        /// </summary>
        public static readonly int SizeInBytes = Vector3.SizeInBytes + Vector2.SizeInBytes + Vector3.SizeInBytes + Vector4.SizeInBytes;
    }
}
