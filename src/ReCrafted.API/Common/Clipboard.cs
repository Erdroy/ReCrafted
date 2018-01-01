// ReCrafted (c) 2016-2018 Always Too Late

using ReCrafted.API.Core;

namespace ReCrafted.API.Common
{
    /// <summary>
    /// Clipboard.
    /// </summary>
    public static class Clipboard
    {
        /// <summary>
        /// Sets the Clipboard text data.
        /// </summary>
        /// <param name="text">Data to set.</param>
        public static void SetText(string text)
        {
            Game.SetClipboard(text);
        }

        /// <summary>
        /// Gets text from Clipboard.
        /// </summary>
        /// <returns>Clipboard text.</returns>
        public static string GetText()
        {
            return Game.GetClipboard();
        }
    }
}
