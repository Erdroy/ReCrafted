// ReCrafted © 2016-2017 Always Too Late

using System;
using ReCrafted.API.Graphics;
using ReCrafted.API.Mathematics;

namespace ReCrafted.API.UI.Controls
{
    /// <summary>
    /// UIText control.
    /// </summary>
    public class UIText : UIControl
    {
        // current text;
        private string _text;

        /// <summary>
        /// Creates new UIText.
        /// </summary>
        public UIText()
        {
            _applyDefaults(new RectangleF(), string.Empty, Color.White);
        }

        /// <summary>
        /// Creates new UIText.
        /// </summary>
        /// <param name="region">Region of the new UIText.</param>
        public UIText(RectangleF region)
        {
            _applyDefaults(region, string.Empty, Color.White);
        }

        /// <summary>
        /// Creates new UIText.
        /// </summary>
        /// <param name="text">Text of the new UIText.</param>
        public UIText(string text)
        {
            _applyDefaults(new RectangleF(), text, Color.White);
        }

        /// <summary>
        /// Creates new UIText.
        /// </summary>
        /// <param name="text">Text of new UIText.</param>
        /// <param name="color">Text color of new UIText.</param>
        public UIText(string text, Color color)
        {
            _applyDefaults(new RectangleF(), text, color);
        }

        /// <summary>
        /// Creates new UIText.
        /// </summary>
        /// <param name="region">Region of the new UIText.</param>
        /// <param name="text">Text of the new UIText.</param>
        public UIText(RectangleF region, string text)
        {
            _applyDefaults(region, text, Color.White);
        }

        /// <summary>
        /// Creates new UIText.
        /// </summary>
        /// <param name="region">Region of the new UIText.</param>
        /// <param name="text">Textr of the new UIText.</param>
        /// <param name="color">Text color of new UIText.</param>
        public UIText(RectangleF region, string text, Color color)
        {
            _applyDefaults(region, text, color);
        }

        // draw text
        public override void Draw()
        {
            if (!Enabled) return;
            TextPosition = new Vector2(Region.X + (Region.Width != 0 ? Region.Width / 2 - TextSize.X / 2 : 0),
                Region.Y + (Region.Height != 0 ? Region.Height / 2 - TextSize.Y / 2 : 0));

            var pos = TextPosition + FixedTextPosition;

            if (EnableClipping)
                UIInternal.BeginViewRect(Region);

            // debug
            // UIInternal.Depth = Depth;
            // UIInternal.Color = Color.Green;
            // UIInternal.DrawBox(Region);

            UIInternal.Color = TextColor;
            UIInternal.DrawString(TextFont.NativePtr, _text, ref pos);

            if (EnableClipping)
                UIInternal.EndViewRect();
        }

        // reset
        public override void Reset()
        {

        }

        // on region changed
        public override void OnRegionChanged()
        {
            base.OnRegionChanged();

            if (FitRegionSizeToText)
            {
                TextSize = TextFont.MeasureString(_text);
                if (FitRegionSizeToText)
                    Region = new RectangleF(Region.X, Region.Y, TextSize.X, TextSize.Y);
            }
        }

        /// <summary>
        /// Updates current font of the text.
        /// </summary>
        /// <param name="font">Our new font.</param>
        /// <exception cref="ArgumentNullException">Exception is thrown when the targetfont was null.</exception>
        public void SetFont(Font font)
        {
            if (font == null) throw new ArgumentNullException(nameof(font));
            TextFont = font;
        }

        // set default properties
        private void _applyDefaults(RectangleF region, string text, Color color)
        {
            IgnoreLayoutResize = true;

            Region = region;
            TextFont = DefaultFont; //set default font
            Text = text;
            TextColor = color;
            FitRegionSizeToText = true;
            EnableClipping = false;
            Enabled = true;
            IgnoreMouseCollision = true;
            IsMouseOver = false;
            Parent = null;

            if (Math.Abs(Region.Height) < 0.01f)
                Region = new RectangleF(Region.X, Region.Y, Region.Width, TextFont.MeasureString(text).Y);

            if (Math.Abs(Region.Width) < 0.01f)
                Region = new RectangleF(Region.X, Region.Y, TextFont.MeasureString(text).X, Region.Height);
        }

        /// <summary>
        /// Color of the text.
        /// </summary>
        public Color TextColor { get; set; }

        /// <summary>
        /// Region size will be automatically updated to current text size.
        /// </summary>
        public bool FitRegionSizeToText { get; set; }

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
                if (FitRegionSizeToText)
                    Region = new RectangleF(Region.X, Region.Y, TextSize.X, TextSize.Y);
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
        /// Fixed text position.
        /// </summary>
        public Vector2 FixedTextPosition { get; set; }

        /// <summary>
        /// Loaded font of this text.
        /// </summary>
        public Font TextFont { get; private set; }

        /// <summary>
        /// Enable text clipping.
        /// </summary>
        public bool EnableClipping { get; set; }
    }
}
