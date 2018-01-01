// ReCrafted (c) 2016-2018 Always Too Late

using System;
using ReCrafted.API.Common;
using ReCrafted.API.Core;
using ReCrafted.API.Mathematics;

namespace ReCrafted.API.UI.Controls
{
    /// <summary>
    /// UI Scroll bar class.
    /// </summary>
    public class UIScrollbar : UIControl
    {
        // current scroll bar handle color
        private Color _color;

        // current scroll bar background color
        private Color _backgroundColor;

        // position of handle on screen
        private float _handlePosition;

        // fixed position of handle
        private float _fixedHandlePosition;

        private RectangleF _handleRegion;

        /// <summary>
        /// Event of scroll bar value (position) change.
        /// </summary>
        public Action<float> OnValueChanged;

        /// <summary>
        /// When scroll bar handle changes his position.
        /// </summary>
        public Action<float> OnHandleChanged;

        /// <summary>
        /// When scroll bar handle has been click by user.
        /// </summary>
        public Action OnHandleClick;

        /// <summary>
        /// Creates new instance of UI Scroll bar.
        /// </summary>
        public UIScrollbar()
        {
            _applyDefaults(new RectangleF(), 0f, 0.5f, UIControlColors.DefaultHandle,
                UIControlColors.DefaultHandleBackground);
        }

        /// <summary>
        /// Creates new instance of UI Scroll bar.
        /// </summary>
        /// <param name="region">Region of new scroll bar.</param>
        public UIScrollbar(RectangleF region)
        {
            _applyDefaults(region, 0f, 0.5f, UIControlColors.DefaultHandle, UIControlColors.DefaultHandleBackground);
        }

        /// <summary>
        /// Creates new instance of UI Scroll bar.
        /// </summary>
        /// <param name="region">Region of new scroll bar.</param>
        /// <param name="position">Position of handle in scroll bar.</param>
        /// <param name="size">Size of handle in scroll bar.</param>
        public UIScrollbar(RectangleF region, float position, float size)
        {
            _applyDefaults(region, position, size, UIControlColors.DefaultHandle,
                UIControlColors.DefaultHandleBackground);
        }

        /// <summary>
        /// Creates new instance of UI Scroll bar.
        /// </summary>
        /// <param name="position">Position of handle in scroll bar.</param>
        /// <param name="size">Size of handle in scroll bar.</param>
        public UIScrollbar(float position, float size)
        {
            _applyDefaults(new RectangleF(), position, size, UIControlColors.DefaultHandle,
                UIControlColors.DefaultHandleBackground);
        }

        /// <summary>
        /// Creates new instance of UI Scroll bar.
        /// </summary>
        /// <param name="region">Region of new scroll bar.</param>
        /// <param name="position">Position of handle in scroll bar.</param>
        /// <param name="size">Size of handle in scroll bar.</param>
        /// <param name="colors">Colors of scroll bar handle.</param>
        /// <param name="backgroundColor">Colors of scroll bar background.</param>
        public UIScrollbar(RectangleF region, float position, float size, UIControlColors colors,
            UIControlColors backgroundColor)
        {
            _applyDefaults(region, position, size, colors, backgroundColor);
        }

        // on mouse enter
        public override void OnMouseEnter()
        {
            if (!SmoothColors)
                _backgroundColor = BackgroundColor.OverColor;
        }

        // on mouse over
        public override void OnMouseOver()
        {
            if (!SmoothColors)
            {
                if (IsDragging)
                    _backgroundColor = BackgroundColor.NormalColor;
                else
                {
                    _backgroundColor = BackgroundColor.OverColor;
                }
            }
        }

        // on mouse exit
        public override void OnMouseExit()
        {
            if (!SmoothColors)
                _backgroundColor = BackgroundColor.NormalColor;
        }

        // draw
        public override void Draw()
        {
            if (IsMouseOver && Input.IsKeyDown(Keys.Mouse0))
            {
                IsDragging = true;
                _handlePosition = Vertical
                    ? Input.CursorPosition.Y - Region.Y + _handleRegion.Height / 2
                    : Input.CursorPosition.X - Region.X + _handleRegion.Width / 2;
                if (Reverse)
                    _handlePosition = Vertical ? Region.Height - _handlePosition : Region.Width - _handlePosition;
                OnValueChanged?.Invoke(HandlePosition);
                OnHandleClick?.Invoke();
            }
            else if (IsDragging && Input.IsKeyUp(Keys.Mouse0))
                IsDragging = false;

            if (IsDragging)
            {
                var previousHandlePosition = _handlePosition;

                var delta = (Vertical ? Input.CursorDelta.Y : Input.CursorDelta.X) *
                            MathUtil.Clamp(HandleSize * 7f, 0.5f, 1f);
                if (Reverse)
                    _handlePosition -= delta;
                else
                {
                    _handlePosition += delta;
                }

                _handlePosition = MathUtil.Clamp(_handlePosition, 0, Vertical ? Region.Height : Region.Width);
                if (Math.Abs(previousHandlePosition - _handlePosition) > 0.05f)
                    OnValueChanged?.Invoke(HandlePosition);

                _color = Color.Lerp(_color, Colors.ClickColor, (float) Time.DeltaTime * SmoothTranslation);
            }
            else
            {
                _color = Color.Lerp(_color, IsMouseOver ? Colors.OverColor : Colors.NormalColor,
                    (float) Time.DeltaTime * SmoothTranslation);
                if (!IsMouseOver && FocusedControl == this)
                    SetFocusedControl(null);
            }

            UIInternal.Depth = Depth;
            _backgroundColor = SmoothColors
                ? Color.Lerp(_backgroundColor,
                    IsDragging
                        ? BackgroundColor.NormalColor
                        : IsMouseOver
                            ? BackgroundColor.OverColor
                            : BackgroundColor.NormalColor, (float) Time.DeltaTime * SmoothTranslation)
                : _backgroundColor;
            UIInternal.Color = _backgroundColor;
            UIInternal.DrawBox(Region);

            UIInternal.Depth = Depth + 0.1f;
            UIInternal.Color = _color;

            HandleSize = MathUtil.Clamp(HandleSize, 0.05f, 1f);

            var pe = _fixedHandlePosition;
            if (Vertical)
            {
                var height = MathUtil.Clamp(Region.Height * HandleSize, Region.Height * 0.05f, Region.Height);

                var min = Region.Y;
                var max = Region.Y + Region.Height - height;
                var hp = Reverse
                    ? Region.Y + Region.Height - _handlePosition - height
                    : Region.Y + _handlePosition - height;
                hp = MathUtil.Clamp(hp, min, max);

                _fixedHandlePosition = MathUtil.Lerp(_fixedHandlePosition, hp, (float) Time.DeltaTime * 15f);

                _handleRegion = new RectangleF(Region.X, _fixedHandlePosition, Region.Width, height);
            }
            else
            {
                var width = MathUtil.Clamp(Region.Width * HandleSize, Region.Width * 0.05f, Region.Width);

                var min = Region.X;
                var max = Region.X + Region.Width - width;
                var hp = Reverse
                    ? Region.X + Region.Width - _handlePosition - width
                    : Region.X + _handlePosition - width;
                hp = MathUtil.Clamp(hp, min, max);

                _fixedHandlePosition = MathUtil.Lerp(_fixedHandlePosition, hp, (float) Time.DeltaTime * 15f);

                _handleRegion = new RectangleF(_fixedHandlePosition, Region.Y, width, Region.Height);
            }

            if (Math.Abs(pe - _fixedHandlePosition) > 0.01f)
            {
                OnHandleChanged?.Invoke(HandlePosition);
            }

            UIInternal.DrawBox(_handleRegion);
        }

        // reset
        public override void Reset()
        {
            _color = Colors.NormalColor;
            _backgroundColor = Colors.NormalColor;
        }

        // set default properties
        private void _applyDefaults(RectangleF region, float position, float size, UIControlColors colors,
            UIControlColors backgroundColors)
        {
            Region = region;
            Enabled = true;
            IgnoreMouseCollision = false;
            IsMouseOver = false;
            Parent = null;

            Colors = colors;
            BackgroundColor = backgroundColors;
            SmoothColors = true;
            SmoothTranslation = 10f;
            Vertical = false;
            HandleSize = size;
            HandlePosition = position;

            _color = Colors.NormalColor;
            _backgroundColor = BackgroundColor.NormalColor;

            IsDragging = false;
            _handlePosition = 0f;
            _fixedHandlePosition = 0f;
        }

        /// <summary>
        /// Colors of scroll bar handle.
        /// </summary>
        public UIControlColors Colors { get; set; }

        /// <summary>
        /// Color of scroll bar background.
        /// </summary>
        public UIControlColors BackgroundColor { get; set; }

        /// <summary>
        /// Colors on toggle will be changed smoothly.
        /// </summary>
        public bool SmoothColors { get; set; }

        /// <summary>
        /// Speed of smooth translation.
        /// </summary>
        public float SmoothTranslation { get; set; }

        /// <summary>
        /// Is scroll bar has to be vertical?
        /// By default is horizontal.
        /// </summary>
        public bool Vertical { get; set; }

        /// <summary>
        /// Position of scroll bar handle will be reversed.
        /// </summary>
        public bool Reverse { get; set; }

        /// <summary>
        /// Current size of scroll bar.
        /// </summary>
        public float HandleSize { get; set; }

        /// <summary>
        /// Current position of scroll bar.
        /// </summary>
        public float HandlePosition
        {
            get
            {
                return Vertical
                    ? Math.Abs((Region.Y - _fixedHandlePosition) / Region.Height)
                    : Math.Abs((Region.X - _fixedHandlePosition) / Region.Width);
            }

            set
            {
                var previousHandlePosition = _handlePosition;
                if (Vertical)
                {
                    _handlePosition = Region.Height * value;
                }
                else
                {
                    _handlePosition = Region.Width * value;
                }

                _handlePosition = MathUtil.Clamp(_handlePosition, 0f, 1f);

                if (Math.Abs(previousHandlePosition - _handlePosition) > 0.05f)
                    OnValueChanged?.Invoke(HandlePosition);
            }
        }

        /// <summary>
        /// Is scroll bar handle currently dragging by user?
        /// </summary>
        public bool IsDragging { get; private set; }
    }
}
