// ReCrafted © 2016-2017 Always Too Late

using System.IO;
using System.Linq;
using Newtonsoft.Json;

namespace ReCrafted.API.Graphics
{
    /// <summary>
    /// Atlas class. Provides basic atlas functionality.
    /// </summary>
    public class Atlas
    {
        /// <summary>
        /// Basic atlas element.
        /// </summary>
        public struct AtlasElement
        {
            public string Name { get; set; }
            public int X { get; set; }
            public int Y { get; set; }
            public int Width { get; set; }
            public int Height { get; set; }
        }

        private AtlasElement[] _elements;

        /// <summary>
        /// Default destructor.
        /// </summary>
        ~Atlas()
        {
            // destroy texture when this object is disposed
            // we must do this to not create any memory leaks
            Object.Destroy(Texture);
        }

        /// <summary>
        /// Gets atlas element by name.
        /// </summary>
        /// <param name="name">The name.</param>
        /// <returns>The found atlas element or null when not found.</returns>
        public AtlasElement GetElement(string name)
        {
            return _elements.FirstOrDefault(x => x.Name == name);
        }

        /// <summary>
        /// Creates new atlas from file.
        /// WARNING: Atlas file name must be the same as the texture file, and must be in the same directory.
        /// </summary>
        /// <param name="fileName">The atlas file name.</param>
        /// <returns>The newly created atlas.</returns>
        public static Atlas Create(string fileName)
        {
            fileName = Path.GetDirectoryName(fileName) + "/" + Path.GetFileNameWithoutExtension(fileName);
            
            var atlasTexture = Texture2D.Create(fileName + ".png"); // TODO: We need to embed the texture file name into JSON
            var elements = JsonConvert.DeserializeObject<AtlasElement[]>(File.ReadAllText(fileName + ".json"));

            return new Atlas
            {
                Texture = atlasTexture,
                _elements = elements
            };
        }

        /// <summary>
        /// The texture of this atlas.
        /// </summary>
        public Texture2D Texture { get; private set; }
    }
}
