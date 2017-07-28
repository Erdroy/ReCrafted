// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

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
        /// Loads Texture2D from PNG file.
        /// </summary>
        /// <param name="fileName">The PNG file relative path (to game executable).</param>
        public void LoadFromFile(string fileName)
        {
            if (!File.Exists(fileName))
            {
                Logger.Write($"Failed to load Texture2D from file '{fileName}'", LogLevel.Error);
                return;
            }

            InternalLoadFile(NativePtr, fileName);
        }
    }
}
