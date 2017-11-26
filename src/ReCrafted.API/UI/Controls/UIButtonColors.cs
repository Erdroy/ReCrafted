// ReCrafted © 2016-2017 Always Too Late

using ReCrafted.API.Mathematics;

namespace ReCrafted.API.UI.Controls
{
    /// <summary>
    /// Colors of the button.
    /// </summary>
    public struct UIButtonColors
    {
        /// <summary>
        /// Normal color of button.
        /// </summary>
        public Color NormalColor { get; set; }

        /// <summary>
        /// Color of button when mouse is over.
        /// </summary>
        public Color OverColor { get; set; }

        /// <summary>
        /// Color of button when button is clicked.
        /// </summary>
        public Color ClickColor { get; set; }

        /// <summary>
        /// Default set of button colors.
        /// </summary>
        public static UIButtonColors Defaults = new UIButtonColors
        {
            NormalColor = Color.White,
            OverColor = Color.Gray,
            ClickColor = Color.DarkGray
        };
    }
}
