// ReCrafted (c) 2016-2018 Always Too Late

using System.IO;
using ReCrafted.API.Core;

namespace ReCrafted.API.Graphics
{
    /// <summary>
    ///	Texture2D class.
    /// </summary>
    public partial class Texture2D
    {
        /// <summary>
        /// Creates new Texture2D from PNG file.
        /// </summary>
        /// <param name="fileName">The PNG file path.</param>
        /// <exception cref="ReCraftedException">Exception is thrown when the target PNG file was unable to load.</exception>
        public static Texture2D Create(string fileName)
        {
            var texture = Create();
            texture.LoadFromFile(fileName);
            texture.Apply();
            return texture;
        }

        /// <summary>
        /// Loads Texture2D from PNG file.
        /// </summary>
        /// <param name="fileName">The PNG file relative path (to game executable).</param>
        /// <exception cref="ReCraftedException">Exception is thrown when the target PNG file was unable to load.</exception>
        public void LoadFromFile(string fileName)
        {
            if (!File.Exists(fileName))
                throw new ReCraftedException($"Failed to load Texture2D from file '{fileName}'");

            InternalLoadFile(NativePtr, fileName);
        }

        /// <summary>
        ///	Uploads all texture changes to GPU
        /// </summary>
        public void Apply()
        {
            InternalApply(NativePtr);
        }
    }
}
