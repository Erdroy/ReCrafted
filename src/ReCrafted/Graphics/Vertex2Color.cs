// ReCrafted © 2016 Damian 'Erdroy' Korczowski

using SharpDX;

namespace ReCrafted.Graphics
{
    /// <summary>
    /// Vertex2 structure.
    /// </summary>
    public struct Vertex2Color
    {
        /// <summary>
        /// Vertice
        /// </summary>
        public Vector3 Vertice;

        /// <summary>
        /// Color
        /// </summary>
        public Vector4 Color;

        /// <summary>
        /// Size in bytes.
        /// </summary>
        public static readonly int SizeInBytes = Vector3.SizeInBytes + Vector4.SizeInBytes;
    }
}
