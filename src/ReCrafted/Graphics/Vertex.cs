// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

using SharpDX;

namespace ReCrafted.Graphics
{
    /// <summary>
    /// Vertex structure.
    /// </summary>
    public struct Vertex
    {
        /// <summary>
        /// Vertice
        /// </summary>
        public Vector3 Vertice;

        /// <summary>
        /// Size in bytes.
        /// </summary>
        public static readonly int SizeInBytes = Vector3.SizeInBytes;
    }
}
