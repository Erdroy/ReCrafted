// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

using ReCrafted.Tools.Common;

namespace ReCrafted.Tools.ContentEditor
{
    /// <summary>
    ///     Settings JSON template class. Provides settings serialization for the application.
    /// </summary>
    public class Settings : JsonTemplate<Settings>
    {
        /// <summary>
        ///     The file name of this JSON template.
        /// </summary>
        public const string FileName = "ContentEditor.Settings.json";

        /// <summary>
        ///     Absolute path to the game's directory.
        /// </summary>
        public string GameDirectory { get; set; }

        /// <summary>
        ///     The current instance of the <see cref="Settings"/> JSON template class.
        /// </summary>
        public static Settings Current { get; set; }
    }
}
