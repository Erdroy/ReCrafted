using SharpDX;

namespace ReCrafted.Graphics
{
    /// <summary>
    /// Vertex2 structure.
    /// </summary>
    public struct Vertex2
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
        /// Size in bytes.
        /// </summary>
        public static readonly int SizeInBytes = Vector3.SizeInBytes + Vector2.SizeInBytes;
    }
}
