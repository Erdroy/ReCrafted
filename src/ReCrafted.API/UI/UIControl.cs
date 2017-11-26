// ReCrafted © 2016-2017 Always Too Late

using ReCrafted.API.Graphics;
using ReCrafted.API.Mathematics;

namespace ReCrafted.API.UI
{
    /// <summary>
    /// UIControl base class.
    /// </summary>
    public abstract class UIControl
    {
        /// <summary>
        /// Default font of all new controls that use font.
        /// </summary>
        public static Font DefaultFont { get; set; }

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

        /// <summary>
        /// When region of control has been changed by his parent.
        /// </summary>
        public virtual void OnRegionChanged() { }

        /// <summary>
        /// Is this control ignore the mouse collision?
        /// </summary>
        public bool IgnoreMouseCollision { get; set; }

        /// <summary>
        /// Is mouse currently over this control?
        /// </summary>
        public bool IsMouseOver { get; internal set; }

        //internal
        public virtual bool OnMouseCollision()
        {
            return true;      
        }

        /// <summary>
        /// When mouse will enter this control.
        /// </summary>
        public virtual void OnMouseEnter() { }

        /// <summary>
        /// When mouse will exit this control.
        /// </summary>
        public virtual void OnMouseExit() { }

        /// <summary>
        /// When mouse is over this control.
        /// </summary>
        public virtual void OnMouseOver() { }

        /// <summary>
        /// When mouse will click this control.
        /// </summary>
        public virtual void OnMouseClick() { }
    }
}
