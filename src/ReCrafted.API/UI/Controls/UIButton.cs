// ReCrafted © 2016-2017 Always Too Late

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

        private UIButton() { }

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

        public override void Draw()
        {
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

        internal override void OnDepthChanged()
        {
            Depth = Parent.Depth + 1;
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

        /// <summary>
        /// Creates new UIButton.
        /// </summary>
        /// <returns>Our newly created UIButton control.</returns>
        public static UIButton Create()
        {
            return Create(new RectangleF(), string.Empty, Color.White, UIButtonColors.Defaults);
        }

        /// <summary>
        /// Creates new UIButton.
        /// </summary>
        /// <param name="text">Text of button.</param>
        /// <returns>Our newly created UIButton control.</returns>
        public static UIButton Create(string text)
        {
            return Create(new RectangleF(), text, Color.White, UIButtonColors.Defaults);
        }

        /// <summary>
        /// Creates new UIButton.
        /// </summary>
        /// <param name="text">Text of button.</param>
        /// <param name="textColor">Color of the button.</param>
        /// <returns>Our newly created UIButton control.</returns>
        public static UIButton Create(string text, Color textColor)
        {
            return Create(new RectangleF(), text, textColor, UIButtonColors.Defaults);
        }

        /// <summary>
        /// Creates new UIButton.
        /// </summary>
        /// <param name="region">The UIButton region.</param>
        /// <returns>Our newly created UIButton control.</returns>
        public static UIButton Create(RectangleF region)
        {
            return Create(region, string.Empty, Color.White, UIButtonColors.Defaults);
        }

        /// <summary>
        /// Creates new UIButton.
        /// </summary>
        /// <param name="region">The UIButton region.</param>
        /// <param name="text">Text of button.</param>
        /// <returns>Our newly created UIButton control.</returns>
        public static UIButton Create(RectangleF region, string text)
        {
            return Create(region, text, Color.White, UIButtonColors.Defaults);
        }

        /// <summary>
        /// Creates new UIButton.
        /// </summary>
        /// <param name="region">The UIButton region.</param>
        /// <param name="text">Text of button.</param>
        /// <param name="textColor">Color of the button.</param>
        /// <returns>Our newly created UIButton control.</returns>
        public static UIButton Create(RectangleF region, string text, Color textColor)
        {
            return Create(region, text, textColor, UIButtonColors.Defaults);
        }

        /// <summary>
        /// Creates new UIButton.
        /// </summary>
        /// <param name="region">The UIButton region.</param>
        /// <param name="text">Text of button.</param>
        /// <param name="textColor">Color of the button.</param>
        /// <param name="colors">Colors of the UIButton.</param>
        /// <returns>Our newly created UIButton control.</returns>
        public static UIButton Create(RectangleF region, string text, Color textColor, UIButtonColors colors)
        {
            return new UIButton
            {
                Region = region,
                TextFont = DefaultFont, //load default font
                Text = text,
                TextColor = textColor,
                _color = Color.White, 
                Colors =  colors,
                SmoothColors = true,
                SmoothTranslation = 5f,
                Enabled = true,
                IgnoreMouseCollision = false,
                IsMouseOver = false,
                Parent = null
            };
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
