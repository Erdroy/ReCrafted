// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

using System.IO;
using Newtonsoft.Json;
using SharpDX;

namespace ReCrafted.Graphics
{
    public class MeshData
    {
        /// <summary>
        /// Vertices
        /// </summary>
        public Vector3[] Positions;

        /// <summary>
        /// Normals
        /// </summary>
        public Vector3[] Normals;

        /// <summary>
        /// UVs
        /// </summary>
        public Vector2[] UVs;

        /// <summary>
        /// Colors
        /// </summary>
        public Color[] Colors;

        /// <summary>
        /// Indices
        /// </summary>
        public uint[] Indices;

        /// <summary>
        /// Load MeshData from file.
        /// </summary>
        /// <param name="meshFile">The file.</param>
        /// <returns>The loaded mesh data.</returns>
        public static MeshData FromFile(string meshFile)
        {
            var sourceFile = "assets/models/" + meshFile + ".json";
            return JsonConvert.DeserializeObject<MeshData>(File.ReadAllText(sourceFile));
        }
    }
}