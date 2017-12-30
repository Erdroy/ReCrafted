// ReCrafted © 2016-2017 Always Too Late

using ReCrafted.API.Mathematics;

namespace ReCrafted.API.UI.Controls
{
    /// <summary>
    /// Colors of the button.
    /// </summary>
    public struct UIControlColors
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
        /// Default set of colors. (For ex. button, background of toggle)
        /// </summary>
        public static UIControlColors Default = new UIControlColors
        {
            NormalColor = Color.White,
            OverColor = Color.Gray,
            ClickColor = Color.DarkGray
        };

        /// <summary>
        /// Default set of check box colors.
        /// </summary>
        public static UIControlColors DefaultCheckBox = new UIControlColors
        {
            NormalColor = Color.Orange,
            OverColor = Color.Orange,
            ClickColor = Color.Orange
        };

        /// <summary>
        /// Default set of colors for ui handle. (For ex. scroll bar)
        /// </summary>
        public static UIControlColors DefaultHandle = new UIControlColors
        {
            NormalColor = Color.DarkGray,
            OverColor = Color.Orange,
            ClickColor = Color.DarkOrange
        };

        /// <summary>
        /// Default set of colors for ui handle background. (For ex. scroll bar)
        /// </summary>
        public static UIControlColors DefaultHandleBackground = new UIControlColors
        {
            NormalColor = Color.White,
            OverColor = Color.White,
            ClickColor = Color.White
        };

        public static UIControlColors Create(Color color)
        {
            return new UIControlColors
            {
                NormalColor = color,
                OverColor = color,
                ClickColor = color
            };
        }
    }
}
