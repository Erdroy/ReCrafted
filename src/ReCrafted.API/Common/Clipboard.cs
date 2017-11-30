// ReCrafted © 2016-2017 Always Too Late

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
    }
}
