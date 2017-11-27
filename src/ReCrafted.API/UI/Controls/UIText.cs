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
        private UIText() { }

        public override void Draw()
        {
            var pos = new Vector2(Region.X, Region.Y);

            UIInternal.Depth = Depth;
            UIInternal.Color = TextColor;
            UIInternal.DrawString(TextFont.NativePtr, Text, ref pos);
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
        /// Creates new UIText.
        /// </summary>
        /// <param name="text">Our text.</param>
        /// <returns>Our newly created UIText control.</returns>
        public static UIText Create(string text)
        {
            return Create(new RectangleF(), text, Color.White);
        }

        /// <summary>
        /// Creates new UIText.
        /// </summary>
        /// <param name="region">The UIText region.</param>
        /// <param name="text">Our text.</param>
        /// <returns>Our newly created UIText control.</returns>
        public static UIText Create(RectangleF region, string text)
        {
            return Create(region, text, Color.White);
        }

        /// <summary>
        /// Creates new UIBox.
        /// </summary>
        /// <param name="region">The UIText region.</param>
        /// <param name="text">Our text.</param>
        /// <param name="color">Color of this box.</param>
        /// <returns>Our newly created UIText control.</returns>
        public static UIText Create(RectangleF region, string text, Color color)
        {
            return new UIText
            {
                Region = region,
                TextFont = DefaultFont, //load default font
                Text = text,
                TextColor = color,
                Enabled = true,
                IgnoreMouseCollision = true,
                IsMouseOver = false,
                Parent = null
            };
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
        public string Text { get; set; }

        /// <summary>
        /// Loaded font of this text.
        /// </summary>
        public Font TextFont { get; private set; }
    }
}
