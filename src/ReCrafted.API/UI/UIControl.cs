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
        // default panel of controls
        private static UIPanel _defaultPanel;

        // control preferred size;
        private Vector2 _preferredSize;

        // control depth
        internal int Depth;

        // is this control ignoring layout size changes?
        internal bool IgnoreLayoutResize;

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
        public virtual void OnControlFocused(bool value)
        {
        }

        /// <summary>
        /// When region of control has been changed by his parent.
        /// </summary>
        public virtual void OnRegionChanged()
        {
        }

        /// <summary>
        /// When mouse will enter this control.
        /// </summary>
        public virtual void OnMouseEnter()
        {
        }

        /// <summary>
        /// When mouse will exit this control.
        /// </summary>
        public virtual void OnMouseExit()
        {
        }

        /// <summary>
        /// When mouse is over this control.
        /// </summary>
        public virtual void OnMouseOver()
        {
        }

        /// <summary>
        /// When mouse will click this control.
        /// </summary>
        public virtual void OnMouseClick()
        {
        }

        // when mouse collision is calculated, every controll have checks his childres (if exists) if they collide with mouse,
        // if so method should return false, and then system will stop search for mouse collision at this point
        internal virtual UIControl OnMouseCollision()
        {
            return null;
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
        /// Initialize defaults of controls.
        /// </summary>
        internal static void Init()
        {
            DefaultFont = Font.Load(Assets.ResolveAssetFilePath(AssetType.Font, "VeraMono.ttf"), 12);

            _defaultPanel = UIPanel.Create(new RectangleF(), UILayoutType.Vertical, "Default");
            _defaultPanel.Layout.ForceExpandWidth = false;
            _defaultPanel.Layout.ForceExpandHeight = false;
            _defaultPanel.ApplyLayout = false;

            _defaultPanel.EnableClipping = false;
        }

        /// <summary>
        /// Creates new panel with control instance.
        /// </summary>
        /// <typeparam name="T">Type of control.</typeparam>
        /// <param name="controlInstance">Control instance.</param>
        public static T CreateControl<T>(T controlInstance) where T : UIControl
        {
            return _defaultPanel.Add(controlInstance);
        }

        /// <summary>
        /// Creates new panel with control instance.
        /// </summary>
        /// <typeparam name="T">Type of control.</typeparam>
        /// <param name="controlInstance">Control instance.</param>
        public static void CreateControl<T>(ref T controlInstance) where T : UIControl
        {
            controlInstance = _defaultPanel.Add(controlInstance);
        }

        /// <summary>
        /// The control's region in pixels.
        /// </summary>
        public RectangleF Region { get; set; }

        /// <summary>
        /// The control's position in pixels.
        /// </summary>
        public Vector2 Position
        {
            get { return new Vector2(Region.X, Region.Y); }
            set { Region = new RectangleF(value.X, value.Y, Region.Width, Region.Height); }
        }

        /// <summary>
        /// The control's size in pixels.
        /// </summary>
        public Vector2 Size
        {
            get { return new Vector2(Region.Width, Region.Height); }
            set { Region = new RectangleF(Region.X, Region.Y, value.X, value.Y); }
        }

        /// <summary>
        /// Preferred size of control is used by force expand by H/V and Dynamic Grid layout type.
        /// </summary>
        public Vector2 PreferredSize
        {
            get { return _preferredSize; }
            set
            {
                _preferredSize = value;
                if (Size == Vector2.Zero)
                    Size = value;
            }
        }

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

        /// <summary>
        /// Sets currently focused control.
        /// </summary>
        /// <param name="control"></param>
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
