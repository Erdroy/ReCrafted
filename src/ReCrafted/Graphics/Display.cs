// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

using ReCrafted.Core;

namespace ReCrafted.Graphics
{
    /// <summary>
    /// Display class.
    /// </summary>
    public static class Display
    {
        /// <summary>
        /// The client width.
        /// </summary>
        public static int ClientWidth => Game.Instance.Form.ClientSize.Width;

        /// <summary>
        /// The client height.
        /// </summary>
        public static int ClientHeight => Game.Instance.Form.ClientSize.Height;
    }
}