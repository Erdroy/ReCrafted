// ReCrafted © 2016-2017 Always Too Late

using System;
using System.Collections.Generic;
using System.Linq;
using ReCrafted.API.Common;
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
            UIInternal.Depth = Depth;
            UIInternal.Color = TextColor;

            var fixedTextRegion = GetFixedTextRegion();
            var pos = new Vector2(fixedTextRegion.X, fixedTextRegion.Y);
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
        /// Fixed text region.
        /// </summary>
        public RectangleF GetFixedTextRegion()
        {
            return new RectangleF(Region.X + (Region.Width != 0 ? Region.Width / 2 - _textsize.X / 2 : 0),
                Region.Y + (Region.Height != 0 ? Region.Height / 2 - _textsize.Y / 2 : 0), _textsize.X, _textsize.Y);
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
        internal static Vector2 ResolveTextRegion(Font textFont, string text)
        {
            var fontSize = textFont.Size;
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

        /*
        // this method returns the character screen position.
        // the method may return an invalid index relative to the actual text
        public static Vector2 GetCharPositionFromIndex(RectangleF textRegion, string text, Font textFont, int index)
        {
            if (string.IsNullOrEmpty(text)) return new Vector2(textRegion.X, textRegion.Y);
            if (index >= text.Length) throw new ArgumentOutOfRangeException(nameof(index), index, null);

            var fontSize = Font.GetSize(textFont.NativePtr);
            var lineSize = new List<int> { 0 };
            int line = 0;
            for (var i = 0; i < text.Length; i++)
            {
                if (text[i] == '\n')
                {
                    line++;
                    lineSize.Add(0);
                }
                else
                {
                    lineSize[line]++;
                    if (i == index)
                       return new Vector2(textRegion.X + (fontSize * (lineSize[line] / (fontSize * 0.142f))), textRegion.Y + (fontSize * 1.142f * (lineSize.Count-1)));
                }
            }

            return new Vector2(textRegion.X, textRegion.Y);
        }

        // this method returns the character index that is closest to the mouse position
        // the method may return an invalid index relative to the actual text
        public static int GetCharIndexFromPosition(RectangleF textRegion, string text, Font textFont)
        {
            if (string.IsNullOrEmpty(text))  return 0;
           
            var textFontSize = Font.GetSize(textFont.NativePtr);
            var characters = new List<List<int>> {new List<int>()};
            int line = 0;
            for (var index = 0; index < text.Length; index++)
            {
                if (text[index] == '\n')
                {
                    line++;
                    characters.Add(new List<int>());
                }
                else
                {
                    characters[line].Add(index);
                }
            }

            var mousePosition = Input.CursorPosition;
            var percent = new Vector2((mousePosition.X - textRegion.X) / textRegion.Width, (mousePosition.Y - textRegion.Y) / textRegion.Height);
            var percentX = (int)Math.Round(textRegion.Width * MathUtil.Clamp(percent.X, 0f, 1f) / textFontSize * 2.284f);
            var percentY = (int)Math.Round(textRegion.Height * MathUtil.Clamp(percent.Y, 0f, 1f) / textFontSize * 2.284f / characters.Count);

            if (percentY >= characters.Count)
            {
                if (percentX >= characters[characters.Count - 1].Count)
                    return characters[characters.Count - 1][characters[characters.Count - 1].Count-1];
                else
                {
                    return characters[characters.Count - 1][percentX];
                }
            }
            else
            {
                if (percentX >= characters[percentY].Count)
                    return characters[percentY][characters[percentY].Count-1];
                else
                {
                    return characters[percentY][percentX];
                }
            }
        }
        */

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
        /// Size of current text.
        /// </summary>
        public Vector2 TextSize => _textsize;

        /// <summary>
        /// Loaded font of this text.
        /// </summary>
        public Font TextFont { get; private set; }
    }
}
