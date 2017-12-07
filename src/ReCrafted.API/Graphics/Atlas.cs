// ReCrafted © 2016-2017 Always Too Late

using System.IO;
using System.Linq;
using Newtonsoft.Json;
using ReCrafted.API.Mathematics;

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
            /// <summary>
            /// Name of atlas element.
            /// </summary>
            public string Name { get; set; }

            /// <summary>
            /// Position X in atlas.
            /// </summary>
            public int X { get; set; }

            /// <summary>
            /// Position Y in atlas.
            /// </summary>
            public int Y { get; set; }

            /// <summary>
            /// Width in atlas.
            /// </summary>
            public int Width { get; set; }

            /// <summary>
            /// Height in atlas.
            /// </summary>
            public int Height { get; set; }

            /// <summary>
            /// UVs of this element.
            /// </summary>
            public RectangleF UVs { get; set; }
        }

        private AtlasElement[] _elements;

        // Destructor makes RC crash.
        /*
        /// <summary>
        /// Default destructor.
        /// </summary>
        ~Atlas()
        {
            // destroy texture when this object is disposed
            // we must do this to not create any memory leaks
            Object.Destroy(Texture);
        }*/

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

            var atlasWidth = atlasTexture.Width;
            var atlasHeight = atlasTexture.Height;
            for (var index = 0; index < elements.Length; index++)
            {
                elements[index].UVs = new RectangleF(elements[index].X / (float)atlasWidth, 
                                                    elements[index].Y / (float)atlasHeight,
                                                    elements[index].Width / (float)atlasWidth,
                                                    elements[index].Height / (float)atlasHeight);
            }

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
