// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

using SharpDX;

namespace ReCrafted.Graphics
{
    /// <summary>
    /// Vertex3 structure.
    /// </summary>
    public struct Vertex3UvNormal
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
        /// Size in bytes.
        /// </summary>
        public static readonly int SizeInBytes = Vector3.SizeInBytes + Vector2.SizeInBytes + Vector3.SizeInBytes/*the color size in bytes*/;
    }
}
