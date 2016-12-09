// ReCrafted © 2016 Damian 'Erdroy' Korczowski

using System.IO;
using Newtonsoft.Json;
using SharpDX;

namespace ReCrafted.Graphics
{
    public class MeshData
    {
        public Vector3[] Positions;
        public Vector3[] Normals;
        public Vector2[] UVs;
        public Color[] Colors;
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