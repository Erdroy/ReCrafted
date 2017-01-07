// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

namespace ReCrafted.Graphics.Primitives
{
    /// <summary>
    /// Base class for primitives;
    /// </summary>
    public abstract class Primitive
    {
        /// <summary>
        /// Create mesh data.
        /// </summary>
        /// <returns>The primitive mesh data.</returns>
        public abstract MeshData GetMeshData();
    }
}