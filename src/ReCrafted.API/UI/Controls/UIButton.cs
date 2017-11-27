// ReCrafted © 2016-2017 Always Too Late

using System;
using ReCrafted.API.Common;
using ReCrafted.API.Core;
using ReCrafted.API.Graphics;
using ReCrafted.API.Mathematics;

namespace ReCrafted.API.UI.Controls
{
    /// <summary>
    /// Delegate for UIButton click event.
    /// </summary>
    public delegate void ButtonClick();

    /// <summary>
    /// UIButton control.
    /// </summary>
    public class UIButton : UIControl
    {
        // current button color
        private Color _color;
        // current button text;
        private string _text;
        // size of button text 
        private Vector2 _textsize;

        /// <summary>
        /// Delegate will be invoked, when button has been clicked.
        /// </summary>
        public ButtonClick OnClick;

        /// <summary>
        /// Creates new UIButton.
        /// </summary>
        public UIButton()
        {
            _applyDefaults(new RectangleF(), string.Empty, Color.Black, UIButtonColors.Defaults);
        }

        /// <summary>
        /// Creates new UIButton.
        /// </summary>
        /// <param name="region">Region of new UIButton.</param>
        public UIButton(RectangleF region)
        {
            _applyDefaults(region, string.Empty, Color.Black, UIButtonColors.Defaults);
        }

        /// <summary>
        /// Creates new UIButton.
        /// </summary>
        /// <param name="text">Text of the new UIButton.</param>
        public UIButton(string text)
        {
            _applyDefaults(new RectangleF(), text, Color.Black, UIButtonColors.Defaults);
        }

        /// <summary>
        /// Creates new UIButton.
        /// </summary>
        /// <param name="text">Text of the new UIButton.</param>
        /// <param name="textColor">Color of the text in new UIButton.</param>
        public UIButton(string text, Color textColor)
        {
            _applyDefaults(new RectangleF(), text, textColor, UIButtonColors.Defaults);
        }

        /// <summary>
        /// Creates new UIButton.
        /// </summary>
        /// <param name="colors">Colors of UIButton.</param>
        public UIButton(UIButtonColors colors)
        {
            _applyDefaults(new RectangleF(), string.Empty, Color.Black, colors);
        }

        /// <summary>
        /// Creates new UIButton.
        /// </summary>
        /// <param name="region">Region of new UIButton.</param>
        /// <param name="text">Text of the new UIButton.</param>
        public UIButton(RectangleF region, string text)
        {
            _applyDefaults(region, text, Color.Black, UIButtonColors.Defaults);
        }

        /// <summary>
        /// Creates new UIButton.
        /// </summary>
        /// <param name="region">Region of new UIButton.</param>
        /// <param name="text">Text of the new UIButton.</param>
        /// <param name="textColor">Color of the text in new UIButton.</param>
        public UIButton(RectangleF region, string text, Color textColor)
        {
            _applyDefaults(region, text, textColor, UIButtonColors.Defaults);
        }

        /// <summary>
        /// Creates new UIButton.
        /// </summary>
        /// <param name="region">Region of new UIButton.</param>
        /// <param name="text">Text of the new UIButton.</param>
        /// <param name="textColor">Color of the text in new UIButton.</param>
        /// <param name="colors">Colors of UIButton.</param>
        public UIButton(RectangleF region, string text, Color textColor, UIButtonColors colors)
        {
            _applyDefaults(region, text, textColor, colors);
        }

        public override void OnMouseEnter()
        {
            if (!SmoothColors)
                _color = Colors.OverColor;
        }

        public override void OnMouseOver()
        {
            if (SmoothColors)
            {
                if (Input.IsKeyUp(Keys.Mouse0))
                    _color = Colors.ClickColor;
            }
            else
            {
                _color = Input.IsKey(Keys.Mouse0) ? Colors.ClickColor : Colors.OverColor;
            }

            if (Input.IsKeyUp(Keys.Mouse0))
                OnClick?.Invoke();
        }

        public override void OnMouseExit()
        {
            if (!SmoothColors)
                _color = Colors.NormalColor;
        }

        // draw button
        public override void Draw()
        {
            if (!Enabled) return;          
            if (SmoothColors) _color = Color.Lerp(_color, IsMouseOver ? Colors.OverColor : Colors.NormalColor, (float)Time.DeltaTime * SmoothTranslation);
            UIInternal.Color = _color;
            UIInternal.Depth = Depth;
            UIInternal.DrawBox(Region);

            UIInternal.Depth = Depth + 0.1f;
            UIInternal.Color = TextColor;
            var buttonTextPositon = new Vector2(Region.X, Region.Y);
            buttonTextPositon.X += Region.Width / 2 - _textsize.X / 2;
            buttonTextPositon.Y += Region.Height / 2 - _textsize.Y / 2;
            UIInternal.DrawString(TextFont.NativePtr, _text, ref buttonTextPositon);
        }

        /// <summary>
        /// Updates current font of the text.
        /// </summary>
        /// <param name="font">Our new font.</param>
        public void SetFont(Font font)
        {
            TextFont = font;
        }

        /// <summary>
        /// Get current font size.
        /// </summary>
        public uint GetSize()
        {
            return Font.GetSize(TextFont.NativePtr);
        }

        // set default properties
        public void _applyDefaults(RectangleF region, string text, Color textColor, UIButtonColors colors)
        {
            Region = region;
            TextFont = DefaultFont; //set default font
            Text = text;
            TextColor = textColor;
            Colors = colors;
            SmoothColors = true;
            SmoothTranslation = 10f;
            Enabled = true;
            IgnoreMouseCollision = false;
            IsMouseOver = false;
            Parent = null;
            _color = Colors.NormalColor;
        }

        /// <summary>
        /// Colors of button.
        /// </summary>
        public UIButtonColors Colors { get; set; }

        /// <summary>
        /// Colors on button will be changed smoothly.
        /// </summary>
        public bool SmoothColors { get; set; }

        /// <summary>
        /// Speed of smooth translation.
        /// </summary>
        public float SmoothTranslation { get; set; }

        /// <summary>
        /// Color of the text.
        /// </summary>
        public Color TextColor { get; set; }

        /// <summary>
        /// Text of this control.
        /// </summary>     
        public string Text
        {
            get { return _text; }
            set
            {
                _text = value;
                _textsize = UIText.ResolveTextRegion(TextFont, _text);
            }
        }

        /// <summary>
        /// Loaded font of this text.
        /// </summary>
        public Font TextFont { get; private set; }
    }
}
