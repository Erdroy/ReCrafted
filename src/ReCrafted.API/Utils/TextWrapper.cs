// ReCrafted (c) 2016-2019 Always Too Late

using System;
using System.Collections.Generic;
using System.Text;
using ReCrafted.API.Graphics;

namespace ReCrafted.API.Utils
{
    /// <summary>
    /// TextWrapper class.
    /// </summary>
    public static class TextWrapper
    {
        /// <summary>
        /// Wraps given text.
        /// </summary>
        /// <param name="text">The text.</param>
        /// <param name="font">The font to be used.</param>
        /// <param name="maxWidth">The target width.</param>
        /// <returns>The wrapped text.</returns>
        public static string Wrap(string text, Font font, float maxWidth)
        {
            float height;
            return Wrap(text, font, maxWidth, out height);
        }

        /// <summary>
        /// Wraps given text.
        /// </summary>
        /// <param name="text">The text.</param>
        /// <param name="font">The font to be used.</param>
        /// <param name="maxWidth">The target width.</param>
        /// <param name="height">The final height.</param>
        /// <returns>The wrapped text.</returns>
        public static string Wrap(string text, Font font, float maxWidth, out float height)
        {
            // As Erdroy is lazy ffs - source: 
            // https://stackoverflow.com/questions/3961278/word-wrap-a-string-in-multiple-lines

            var lines = text.Split(new[] { " " }, StringSplitOptions.None);
            var wrappedLines = new List<string>();
            var currentLine = new StringBuilder();
            var currentWidth = 0.0f;

            foreach (var item in lines)
            {
                var w = font.MeasureString(item + " ").X;
                currentWidth += w;

                if (currentWidth > maxWidth)
                {
                    wrappedLines.Add(currentLine.ToString());
                    currentLine.Clear();
                    currentWidth = w;
                }

                currentLine.Append(item + " ");
            }

            if (currentLine.Length > 0)
                wrappedLines.Add(currentLine.ToString());

            var finalText = string.Join("\n", wrappedLines);
            height = font.MeasureString(finalText).Y;
            return finalText;
        }
    }
}
