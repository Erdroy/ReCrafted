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

        // button size for spring animation
        private Vector2 _buttonSize;
        // velocity of button spring animation
        private Vector2 _buttonVelocity;

        // button will ignore next click?
        private bool _ignoreClick;

        /// <summary>
        /// Delegate will be invoked, when button has been clicked.
        /// </summary>
        public ButtonClick OnClick;

        /// <summary>
        /// Creates new UIButton.
        /// </summary>
        public UIButton()
        {
            _applyDefaults(new RectangleF(), string.Empty, Color.Black, UIControlColors.Defaults, null);
        }

        /// <summary>
        /// Creates new UIButton.
        /// </summary>
        /// <param name="region">Region of new UIButton.</param>
        public UIButton(RectangleF region)
        {
            _applyDefaults(region, string.Empty, Color.Black, UIControlColors.Defaults, null);
        }

        /// <summary>
        /// Creates new UIButton.
        /// </summary>
        /// <param name="text">Text of the new UIButton.</param>
        public UIButton(string text)
        {
            _applyDefaults(new RectangleF(), text, Color.Black, UIControlColors.Defaults, null);
        }

        /// <summary>
        /// Creates new UIButton.
        /// </summary>
        /// <param name="text">Text of the new UIButton.</param>
        /// <param name="textColor">Color of the text in new UIButton.</param>
        public UIButton(string text, Color textColor)
        {
            _applyDefaults(new RectangleF(), text, textColor, UIControlColors.Defaults, null);
        }

        /// <summary>
        /// Creates new UIButton.
        /// </summary>
        /// <param name="region">Region of new UIButton.</param>
        /// <param name="text">Text of the new UIButton.</param>
        public UIButton(RectangleF region, string text)
        {
            _applyDefaults(region, text, Color.Black, UIControlColors.Defaults, null);
        }

        /// <summary>
        /// Creates new UIButton.
        /// </summary>
        /// <param name="region">Region of new UIButton.</param>
        /// <param name="text">Text of the new UIButton.</param>
        /// <param name="textColor">Color of the text in new UIButton.</param>
        public UIButton(RectangleF region, string text, Color textColor)
        {
            _applyDefaults(region, text, textColor, UIControlColors.Defaults, null);
        }

        /// <summary>
        /// Creates new UIButton.
        /// </summary>
        /// <param name="region">Region of new UIButton.</param>
        /// <param name="text">Text of the new UIButton.</param>
        /// <param name="textColor">Color of the text in new UIButton.</param>
        /// <param name="colors">Colors of UIButton.</param>
        public UIButton(RectangleF region, string text, Color textColor, UIControlColors colors)
        {
            _applyDefaults(region, text, textColor, colors, null);
        }

        /// <summary>
        /// Creates new UIButton.
        /// </summary>
        /// <param name="region">Region of new UIButton.</param>
        /// <param name="text">Text of the new UIButton.</param>
        /// <param name="textColor">Color of the text in new UIButton.</param>
        /// <param name="colors">Colors of UIButton.</param>
        /// <param name="texture">Texture of button.</param>
        public UIButton(RectangleF region, string text, Color textColor, UIControlColors colors, Texture2D texture)
        {
            _applyDefaults(region, text, textColor, colors, texture);
        }

        public override void OnMouseEnter()
        {
            _ignoreClick = false;
            if (Input.IsKey(Keys.Mouse0))
                _ignoreClick = true;

            if (!SmoothColors)
                _color = Colors.OverColor;
        }

        public override void OnMouseOver()
        {
            if (_ignoreClick && Input.IsKeyDown(Keys.Mouse0))
                _ignoreClick = false;

            if (SmoothColors)
            {
                if (Input.IsKeyUp(Keys.Mouse0))
                    _color = Colors.ClickColor;
            }
            else
            {
                _color = Input.IsKey(Keys.Mouse0) ? Colors.ClickColor : Colors.OverColor;
            }

            if (!_ignoreClick && Input.IsKeyUp(Keys.Mouse0))
            {
                if (SpringAnimation)
                {
                    _buttonVelocity = Vector2.Zero;
                    _buttonSize = new Vector2(Region.Width, Region.Height) * 0.8f;
                }

                OnClick?.Invoke();
            }
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

            UIInternal.Depth = Depth;
            UIInternal.Color = _color;
            var buttonRegion = Region;
            if (SpringAnimation)
            {
                var target = new Vector2(buttonRegion.Width, buttonRegion.Height);
                UIAnimation.SpringVector2(ref _buttonSize, ref _buttonVelocity, target, (float)Time.DeltaTime);
                buttonRegion = new RectangleF(buttonRegion.X - (_buttonSize.X/2f - buttonRegion.Width/2f),
                                              buttonRegion.Y - (_buttonSize.Y/2f - buttonRegion.Height/2f), 
                                              _buttonSize.X, _buttonSize.Y);
            }
            UIInternal.Depth = Depth + 0.1f;
            UIInternal.DrawBox(buttonRegion);
            //if (Texture == null)
            //    UIInternal.DrawBox(buttonRegion);
            //else
            if (Texture != null)
            {
                UIInternal.Depth = Depth + 0.2f;
                UIInternal.Color = _color * 1.5f;
                var uvs = new RectangleF(0f, 0f, 1f, 1f);
                UIInternal.DrawTexture2D(Texture.NativePtr, ref buttonRegion, ref uvs);
            }

            TextPosition = new Vector2(Region.X + Region.Width / 2 - TextSize.X / 2, Region.Y + Region.Height / 2 - TextSize.Y / 2);
            var pos = TextPosition;
            UIInternal.Depth = Depth + 0.2f;
            UIInternal.Color = TextColor;
            UIInternal.DrawString(TextFont.NativePtr, _text, ref pos);
        }

        public override void Reset()
        {
            _color = Colors.NormalColor;
            _buttonSize = new Vector2(Region.Width, Region.Height);
            _buttonVelocity = Vector2.Zero;
        }

        /// <summary>
        /// Updates current font of the text.
        /// </summary>
        /// <param name="font">Our new font.</param>
        /// <exception cref="ArgumentNullException">Exception is thrown when the target font was null.</exception>
        public void SetFont(Font font)
        {
            if (font == null) throw new ArgumentNullException(nameof(font));
            TextFont = font;
        }

        // set default properties
        private void _applyDefaults(RectangleF region, string text, Color textColor, UIControlColors colors, Texture2D texture)
        {
            Region = region;
            TextFont = DefaultFont; //set default font
            Text = text;
            TextColor = textColor;
            Colors = colors;
            SmoothColors = true;
            SmoothTranslation = 10f;
            SpringAnimation = true;
            Texture = texture;
            Enabled = true;
            IgnoreMouseCollision = false;
            IsMouseOver = false;
            Parent = null;

            _color = Colors.NormalColor;
            _buttonSize = new Vector2(Region.Width, Region.Height);
            _buttonVelocity = Vector2.Zero;
        }

        /// <summary>
        /// Colors of button.
        /// </summary>
        public UIControlColors Colors { get; set; }

        /// <summary>
        /// Colors on button will be changed smoothly.
        /// </summary>
        public bool SmoothColors { get; set; }

        /// <summary>
        /// Speed of smooth translation.
        /// </summary>
        public float SmoothTranslation { get; set; }

        /// <summary>
        /// Is button currently using spring animation.
        /// </summary>
        public bool SpringAnimation { get; set; }

        /// <summary>
        /// Color of the text.
        /// </summary>
        public Color TextColor { get; set; }

        /// <summary>
        /// Texture of button.
        /// </summary>
        public Texture2D Texture { get; set; }

        /// <summary>
        /// Text of this control.
        /// </summary>     
        public string Text
        {
            get { return _text; }
            set
            {
                _text = value;
                TextSize = TextFont.MeasureString(_text);
            }
        }

        /// <summary>
        /// Size of current text.
        /// </summary>
        public Vector2 TextSize { get; private set; }

        /// <summary>
        /// Fixed text position.
        /// </summary>
        public Vector2 TextPosition { get; private set; }

        /// <summary>
        /// Loaded font of this text.
        /// </summary>
        public Font TextFont { get; private set; }
    }
}
