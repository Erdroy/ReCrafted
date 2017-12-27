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
        // control depth
        internal int Depth;

        /// <summary>
        /// Draws all controls.
        /// </summary>
        public abstract void Draw();

        /// <summary>
        /// Resets control to the default state.
        /// </summary>
        public abstract void Reset();

        /// <summary>
        /// When focus state of control has been changed.
        /// </summary>
        /// <param name="value"></param>
        public virtual void OnControlFocused(bool value) { }

        /// <summary>
        /// When region of control has been changed by his parent.
        /// </summary>
        public virtual void OnRegionChanged() { }

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

        // when mouse collision is calculated, every controll have checks his childres (if exists) if they collide with mouse,
        // if so method should return false, and then system will stop search for mouse collision at this point
        internal virtual UIControl OnMouseCollision() { return null; }

        /// <summary>
        /// Sets this control as current focus target.
        /// </summary>
        public void SetControlFocused()
        {
            SetFocusedControl(this);
        }

        /// <summary>
        /// Gets root of this control. (In most cases this will be UIPanel)
        /// </summary>
        public UIControl GetRoot()
        {
            var c = Parent;
            var p = Parent;
            while (c != null)
            {
                c = c.Parent;
                if (c != null)
                    p = c;
            }
            return p;
        }

        /// <summary>
        /// The control's region in pixels.
        /// </summary>
        public RectangleF Region { get; set; }

        /// <summary>
        /// Control enable state.
        /// </summary>
        public bool Enabled { get; set; }

        /// <summary>
        /// Is this control ignore the mouse collision?
        /// </summary>
        public bool IgnoreMouseCollision { get; set; }

        /// <summary>
        /// Name of control. (This property might be used in profiling)
        /// </summary>
        public string Name { get; set; }

        /// <summary>
        /// The parent control.
        /// </summary>
        public UIControl Parent { get; internal set; }

        /// <summary>
        /// Is mouse currently over this control?
        /// </summary>
        public bool IsMouseOver { get; internal set; }

        /// <summary>
        /// Is this control currently focused? (if possible)
        /// </summary>
        public bool IsFocused => FocusedControl == this;

        /// <summary>
        /// Default font of all new controls that uses font.
        /// </summary>
        public static Font DefaultFont { get; internal set; }

        // current focused control by user
        internal static UIControl FocusedControl { get; private set; }

        // set focused control
        internal static void SetFocusedControl(UIControl control)
        {
            if (control == null)
                FocusedControl?.OnControlFocused(false);
            var same = FocusedControl == control;
            FocusedControl = control;
            if (FocusedControl != null && !same)
                FocusedControl.OnControlFocused(true);
        }
    }
}
