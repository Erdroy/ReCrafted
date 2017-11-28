// ReCrafted © 2016-2017 Always Too Late

using System;
using ReCrafted.API.Mathematics;

namespace ReCrafted.API.Graphics
{
    /// <summary>
    /// Sprite class.
    /// </summary>
    public class Sprite
    {
        private struct BorderElement
        {
            public Vector2 Position { get; set; }
            public RectangleF UVs { get; set; }
        }

        private BorderElement[] _borderElements;

        private Sprite() { }

        public void Draw()
        {
            throw new NotImplementedException();
        }

        private void CalculateBorders(int radius)
        {
            _borderElements = new BorderElement[9];
            throw new NotImplementedException();
        }
        
        public static Sprite Create(Vector2 position, Texture2D texture)
        {
            throw new NotImplementedException();
        }

        public static Sprite CreateBordered(Vector2 position, Texture2D texture, int borderRadius = 10)
        {
            var sprite = Create(position, texture);
            sprite.CalculateBorders(borderRadius);
            throw new NotImplementedException();
        }

        public Vector2 Position { get; set; }

        public RectangleF UVs { get; set; }

        public Texture2D Texture { get; set; }
    }
}
