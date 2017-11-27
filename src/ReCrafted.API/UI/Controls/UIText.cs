// ReCrafted © 2016-2017 Always Too Late

using System;
using System.Collections.Generic;
using System.Linq;
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
        // size of text 
        private Vector2 _textsize;

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
            var pos = new Vector2(Region.X, Region.Y);
            if (Region.Width != 0 && Region.Height != 0)
            {
                pos.X += Region.Width / 2 - _textsize.X / 2;
                pos.Y += Region.Height / 2 - _textsize.Y / 2;
            }

            UIInternal.Depth = Depth;
            UIInternal.Color = TextColor;
            UIInternal.DrawString(TextFont.NativePtr, _text, ref pos);
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
        private void _applyDefaults(RectangleF region, string text, Color color)
        {
            Region = region;
            TextFont = DefaultFont;//set default font
            Text = text;
            TextColor = color;
            Enabled = true;
            IgnoreMouseCollision = true;
            IsMouseOver = false;
            Parent = null;
        }

        // creates new rect based on given text and font size
        // the method may return an invalid size relative to the actual text
        internal static Vector2 ResolveTextRegion(Font font, string text)
        {
            var fontSize = Font.GetSize(font.NativePtr);
            var lineSize = new List<int> { 0 };
            int line = 0;
            foreach (var t in text)
            {
                if (t == '\n')
                {
                    line++;
                    lineSize.Add(0);
                }
                else
                {
                    lineSize[line]++;
                }
            }
            return new Vector2(fontSize * (lineSize.Max() / (fontSize * 0.142f)), fontSize * 1.142f * lineSize.Count);
        }

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
