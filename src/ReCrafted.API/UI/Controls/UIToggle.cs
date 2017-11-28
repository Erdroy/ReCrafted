// ReCrafted © 2016-2017 Always Too Late

using ReCrafted.API.Common;
using ReCrafted.API.Core;
using ReCrafted.API.Graphics;
using ReCrafted.API.Mathematics;

namespace ReCrafted.API.UI.Controls
{
    /// <summary>
    /// Delegate for toggle value change event.
    /// </summary>
    public delegate void ToggleValueChanged(bool value);

    /// <summary>
    /// UIToggle control.
    /// </summary>
    public class UIToggle : UIControl
    {
        // current toggle background color
        private Color _color;
        // current toggle text;
        private string _text;
        // size of toggle text 
        private Vector2 _textsize;

        // current check box color
        private Color _checkBoxColor;
        // check box size for spring animation
        private Vector2 _checkBoxSize;
        // velocity of check box spring animation
        private Vector2 _checkBoxVelocity;

        // is toggle checked?
        private bool _isOn;

        /// <summary>
        /// Is toggle already checked?
        /// </summary>
        public bool IsOn
        {
            get { return _isOn; }
            set
            {
                _isOn = value;
                _checkBoxColor = _isOn ? CheckBoxColor : Color.Transparent;
            }
        }

        /// <summary>
        /// When value of toggle has been changed by user.
        /// </summary>
        public ToggleValueChanged OnValueChanged;

        /// <summary>
        /// Creates new UIToggle.
        /// </summary>
        public UIToggle()
        {
            _applyDefaults(new RectangleF(), string.Empty, Color.White, UIControlColors.Defaults, Color.Orange);
        }

        /// <summary>
        /// Creates new UIToggle.
        /// </summary>
        /// <param name="region">Region of the new UIToggle.</param>
        public UIToggle(RectangleF region)
        {
            _applyDefaults(region, string.Empty, Color.White, UIControlColors.Defaults, Color.Orange);
        }

        /// <summary>
        /// Creates new UIToggle.
        /// </summary>
        /// <param name="text">Text of the new UIToggle.</param>
        public UIToggle(string text)
        {
            _applyDefaults(new RectangleF(), text, Color.White, UIControlColors.Defaults, Color.Orange);
        }

        /// <summary>
        /// Creates new UIToggle.
        /// </summary>
        /// <param name="text">Text of the new UIToggle.</param>
        /// <param name="textColor">Color of text in the new UIToggle.</param>
        public UIToggle(string text, Color textColor)
        {
            _applyDefaults(new RectangleF(), text, textColor, UIControlColors.Defaults, Color.Orange);
        }

        /// <summary>
        /// Creates new UIToggle.
        /// </summary>
        /// <param name="region">Region of the new UIToggle.</param>
        /// <param name="text">Text of the new UIToggle.</param>
        public UIToggle(RectangleF region, string text)
        {
            _applyDefaults(region, text, Color.White, UIControlColors.Defaults, Color.Orange);
        }

        /// <summary>
        /// Creates new UIToggle.
        /// </summary>
        /// <param name="region">Region of the new UIToggle.</param>
        /// <param name="text">Text of the new UIToggle.</param>
        /// <param name="textColor">Color of text in the new UIToggle.</param>
        public UIToggle(RectangleF region, string text, Color textColor)
        {
            _applyDefaults(region, text, textColor, UIControlColors.Defaults, Color.Orange);
        }

        /// <summary>
        /// Creates new UIToggle.
        /// </summary>
        /// <param name="region">Region of the new UIToggle.</param>
        /// <param name="text">Text of the new UIToggle.</param>
        /// <param name="textColor">Color of text in the new UIToggle.</param>
        /// <param name="colors">Colors of the new UIToggle.</param>
        public UIToggle(RectangleF region, string text, Color textColor, UIControlColors colors)
        {
            _applyDefaults(region, text, textColor, colors, Color.Orange);
        }

        /// <summary>
        /// Creates new UIToggle.
        /// </summary>
        /// <param name="region">Region of the new UIToggle.</param>
        /// <param name="text">Text of the new UIToggle.</param>
        /// <param name="textColor">Color of text in the new UIToggle.</param>
        /// <param name="colors">Colors of the new UIToggle.</param>
        /// <param name="checkBoxColor">Color of checkbox in new UIToggle.</param>
        public UIToggle(RectangleF region, string text, Color textColor, UIControlColors colors, Color checkBoxColor)
        {
            _applyDefaults(region, text, textColor, colors, checkBoxColor);
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
            {
                IsOn = !_isOn;
                OnValueChanged?.Invoke(_isOn);
            }
        }

        public override void OnMouseExit()
        {
            if (!SmoothColors)
                _color = Colors.NormalColor;
        }

        // draw toggle
        public override void Draw()
        {
            if (!Enabled) return;
            if (SmoothColors)
                _color = Color.Lerp(_color, IsMouseOver ? Colors.OverColor : Colors.NormalColor, (float)Time.DeltaTime * SmoothTranslation);
            UIInternal.Depth = Depth;
            UIInternal.Color = TextBackgroundColor;
            UIInternal.DrawBox(Region);

            UIInternal.Color = _color;
            var backgrounRegion = new RectangleF(Region.X, Region.Y, Region.Height, Region.Height);
            UIInternal.DrawBox(backgrounRegion);

            UIInternal.Color = _checkBoxColor;
            var checkBoxRegion = backgrounRegion;
            var target = new Vector2(checkBoxRegion.Width, checkBoxRegion.Height) * (_isOn ? 0.8f : 0.6f);
            UIAnimation.SpringVector2(ref _checkBoxSize, ref _checkBoxVelocity, target, (float)Time.DeltaTime);
            checkBoxRegion = new RectangleF(checkBoxRegion.X - (_checkBoxSize.X / 2f - checkBoxRegion.Width / 2f),
                                            checkBoxRegion.Y - (_checkBoxSize.Y / 2f - checkBoxRegion.Height / 2f),
                                           _checkBoxSize.X, _checkBoxSize.Y);            
            UIInternal.DrawBox(checkBoxRegion);

            UIInternal.Color = TextColor;
            var toggleTextPositon = new Vector2(Region.X, Region.Y);
            toggleTextPositon.X += Region.Height * 1.2f;
            toggleTextPositon.Y += Region.Height / 2 - _textsize.Y / 2;
            UIInternal.DrawString(TextFont.NativePtr, _text, ref toggleTextPositon);
        }
        
        // set default properties
        private void _applyDefaults(RectangleF region, string text, Color textColor, UIControlColors colors, Color checkBoxColor)
        {
            Region = region;
            TextFont = DefaultFont; //set default font
            Text = text;
            TextColor = textColor;
            TextBackgroundColor = Color.Transparent;
            Colors = colors;
            SmoothColors = true;
            SmoothTranslation = 10f;
            CheckBoxColor = checkBoxColor;
            Enabled = true;
            IgnoreMouseCollision = false;
            IsMouseOver = false;
            Parent = null;

            _color = Colors.NormalColor;
            _checkBoxSize = new Vector2(region.Height, region.Height) * 0.8f;
            _checkBoxVelocity = Vector2.One;
            _checkBoxColor = Color.Transparent;
        }

        /// <summary>
        /// Colors of button.
        /// </summary>
        public UIControlColors Colors { get; set; }

        /// <summary>
        /// Color of checkbox in toggle.
        /// </summary>
        public Color CheckBoxColor { get; set; }

        /// <summary>
        /// Colors on toggle will be changed smoothly.
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
        /// Color of the background of the text.
        /// </summary>
        public Color TextBackgroundColor { get; set; }

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
