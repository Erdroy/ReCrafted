using SharpDX;

namespace ReCrafted.Graphics
{
    /// <summary>
    /// Vertex3 structure.
    /// </summary>
    public struct Vertex3
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
        public Color Color;

        /// <summary>
        /// Size in bytes.
        /// </summary>
        public static readonly int SizeInBytes = Vector3.SizeInBytes + Vector2.SizeInBytes + 4/*the color size in bytes*/;
    }
}
