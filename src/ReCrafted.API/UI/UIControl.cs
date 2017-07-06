// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

using ReCrafted.API.Core;

namespace ReCrafted.API.UI
{
    /// <summary>
    /// UIControl base class.
    /// </summary>
    public abstract class UIControl
    {
        /// <summary>
        /// Draws all controls.
        /// </summary>
        public abstract void Draw();

        /// <summary>
        /// The parent control.
        /// </summary>
        public UIControl Parent { get; internal set; }

        /// <summary>
        /// The control's region in pixels.
        /// </summary>
        public RectangleF Region { get; set; }

        /// <summary>
        /// Control enable state.
        /// </summary>
        public bool Enabled { get; set; }
    }
}
