// ReCrafted © 2016-2017 Always Too Late

using ReCrafted.API.Common;
using ReCrafted.API.Core;
using ReCrafted.API.Graphics;
using ReCrafted.API.Mathematics;

namespace ReCrafted.API.UI.Controls
{
    /// <summary>
    /// UITextField control.
    /// </summary>
    public class UITextField : UIControl
    {
        // current textfield color
        private Color _color;
        // current textfield text;
        private string _text;
        // size of textfield text 
        private Vector2 _textsize;

        // color of textField edit box.
        private Color _textFieldEditBoxColor;
        // textField edit box size for spring animation
        private Vector2 _textFieldEditBoxSize;
        // velocity of textField edit box spring animation
        private Vector2 _textFieldEditBoxVelocity;

        /// <summary>
        /// Creates new UITextField.
        /// </summary>
        public UITextField()
        {
            _applyDefaults(new RectangleF(), string.Empty, Color.Black, UIControlColors.Defaults);
        }

        /// <summary>
        /// Creates new UITextField.
        /// </summary>
        /// <param name="region">Region of new UITextField.</param>
        public UITextField(RectangleF region)
        {
            _applyDefaults(region, string.Empty, Color.Black, UIControlColors.Defaults);
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
        }

        public override void OnMouseExit()
        {
            if (!SmoothColors)
                _color = Colors.NormalColor;
        }

        public override void OnControlFocused(bool value)
        {
            if (!SmoothColors)
                _textFieldEditBoxColor = value ? Colors.ClickColor : Color.Transparent;
        }

        public override void Draw()
        {
            if (!Enabled) return;
            if (SmoothColors) _color = Color.Lerp(_color, IsMouseOver ? Colors.OverColor : Colors.NormalColor, (float)Time.DeltaTime * SmoothTranslation);

            UIInternal.Color = _color;
            UIInternal.Depth = Depth;
            UIInternal.DrawBox(Region);

            var buttonRegion = Region;    
            var target = new Vector2(buttonRegion.Width, buttonRegion.Height) * (IsFocused ? 1f : 0.5f);
            UIAnimation.SpringVector2(ref _textFieldEditBoxSize, ref _textFieldEditBoxVelocity, target, (float)Time.DeltaTime);
            buttonRegion = new RectangleF(buttonRegion.X - (_textFieldEditBoxSize.X / 2f - buttonRegion.Width / 2f),
                                          buttonRegion.Y - (_textFieldEditBoxSize.Y / 2f - buttonRegion.Height / 2f),
                                          _textFieldEditBoxSize.X, _textFieldEditBoxSize.Y);

            if (SmoothColors) _textFieldEditBoxColor = Color.Lerp(_textFieldEditBoxColor, IsFocused ? Colors.ClickColor : Color.Transparent, (float) Time.DeltaTime * SmoothTranslation * (IsFocused ? 1f : 2f));
            UIInternal.Color = _textFieldEditBoxColor;
            UIInternal.DrawBox(buttonRegion);

            UIInternal.Depth = Depth + 0.1f;
            UIInternal.Color = TextColor;
            var textPosition = new Vector2(Region.X, Region.Y);
            textPosition.X += Region.Width / 2 - _textsize.X / 2;
            textPosition.Y += Region.Height / 2 - _textsize.Y / 2;
            UIInternal.DrawString(TextFont.NativePtr, _text, ref textPosition);
        }

        // set default properties
        private void _applyDefaults(RectangleF region, string text, Color textColor, UIControlColors colors)
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
            _textFieldEditBoxColor = Color.Transparent;
            _textFieldEditBoxSize = new Vector2(region.Width, region.Height);
            _textFieldEditBoxVelocity = Vector2.One;
        }

        /// <summary>
        /// Colors of textfield.
        /// </summary>
        public UIControlColors Colors { get; set; }

        /// <summary>
        /// Colors on textfield will be changed smoothly.
        /// </summary>
        public bool SmoothColors { get; set; }

        /// <summary>
        /// Speed of smooth color translation.
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
        public Font TextFont { get; set; }
    }
}
