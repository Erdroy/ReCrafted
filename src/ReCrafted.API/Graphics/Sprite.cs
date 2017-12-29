// ReCrafted © 2016-2017 Always Too Late

using ReCrafted.API.Mathematics;
using ReCrafted.API.UI;

namespace ReCrafted.API.Graphics
{
    /// <summary>
    /// Sprite class.
    /// </summary>
    public class Sprite
    {
        /// <summary>
        /// Element of sprite border.
        /// </summary>
        private struct BorderElement
        {
            public RectangleF Region { get; set; }
            public RectangleF UVs { get; set; }
        }

        // elements of sprite border
        private BorderElement[] _borderElements;

        // last region of displayed sprite
        private RectangleF _lastRegion;

        // private constructor
        private Sprite()
        {
            
        }

        // draw
        internal void Draw(RectangleF spriteRegion)
        {
            if (BorderRadius == 0)
            {
                if (Texture != null)
                {
                    var uv = UVs;
                    UIInternal.DrawTexture2D(Texture.NativePtr, ref spriteRegion, ref uv);
                }
                else
                {
                    UIInternal.DrawBox(spriteRegion);
                }
            }
            else
            {
                if (_lastRegion != spriteRegion || _borderElements == null)
                {
                    _lastRegion = spriteRegion;

                    // recalculate border if changed
                    CalculateBorders(spriteRegion);
                }

                foreach (var b in _borderElements)
                {
                    var uv = b.UVs;
                    var region = b.Region;
                    UIInternal.DrawTexture2D(Texture.NativePtr, ref region, ref uv);
                }
            }
        }

        // calculate 9-slice
        private void CalculateBorders(RectangleF region)
        {
            _borderElements = new BorderElement[9];

            var rw = BorderRadius / (float) Texture.Width;
            var rh = BorderRadius / (float) Texture.Height;
            // x--
            // ---
            // ---
            _borderElements[0].UVs = new RectangleF(0.0f, 0.0f, rw, rh);
            _borderElements[0].Region = new RectangleF(region.X, region.Y, BorderRadius, BorderRadius);

            // -x-
            // ---
            // ---
            _borderElements[1].UVs = new RectangleF(rw, 0.0f, 1f - rw * 3f, rh);
            _borderElements[1].Region = new RectangleF(region.X + BorderRadius, region.Y, region.Width - BorderRadius * 2, BorderRadius);

            // --x
            // ---
            // ---
            _borderElements[2].UVs = new RectangleF(1f - rw * 3.5f, 0.0f, rw, rh);
            _borderElements[2].Region = new RectangleF(region.X + region.Width - BorderRadius, region.Y, BorderRadius, BorderRadius);

            // ---
            // x--
            // ---
            _borderElements[3].UVs = new RectangleF(0, rh, rw, 1f - rh * 3f);
            _borderElements[3].Region = new RectangleF(region.X, region.Y + BorderRadius, BorderRadius, region.Height - BorderRadius * 2f);

            // ---
            // -x-
            // ---
            _borderElements[4].UVs = new RectangleF(rw, rh, 1f - rw * 3f, 1f - rh * 3f);
            _borderElements[4].Region = new RectangleF(region.X + BorderRadius, region.Y + BorderRadius, region.Width - BorderRadius * 2f, region.Height - BorderRadius * 2f);

            // ---
            // --x
            // ---
            _borderElements[5].UVs = new RectangleF(1f - rw * 3.5f, rh, rw, 1f - rh * 3f);
            _borderElements[5].Region = new RectangleF(region.X + region.Width - BorderRadius, region.Y + BorderRadius, BorderRadius, region.Height - BorderRadius * 2f);

            // ---
            // ---
            // x--
            _borderElements[6].UVs = new RectangleF(0.0f, 1f - rh * 3.5f, rw, rh);
            _borderElements[6].Region = new RectangleF(region.X, region.Y + region.Height - BorderRadius, BorderRadius, BorderRadius);

            // ---
            // ---
            // -x-
            _borderElements[7].UVs = new RectangleF(rw, 1f - rh * 3.5f, 1f - rw * 3f, rh);
            _borderElements[7].Region = new RectangleF(region.X + BorderRadius, region.Y + +region.Height - BorderRadius, region.Width - BorderRadius * 2, BorderRadius);

            // ---
            // ---
            // --x
            _borderElements[8].UVs = new RectangleF(1f - rw * 3.5f, 1f - rh * 3.5f, rw, rh);
            _borderElements[8].Region = new RectangleF(region.X + region.Width - BorderRadius, region.Y + +region.Height - BorderRadius, BorderRadius, BorderRadius);
        }

        /// <summary>
        /// Creates new sprite instance.
        /// </summary>
        /// <param name="texturePath">Texture of sprite.</param>
        /// <returns>Created instance of sprite.</returns>
        public static Sprite Create(string texturePath)
        {
            return Create(Texture2D.Create(texturePath));
        }

        /// <summary>
        /// Creates new sprite instance.
        /// </summary>
        /// <param name="texture">Texture of sprite.</param>
        /// <returns>Created instance of sprite.</returns>
        public static Sprite Create(Texture2D texture)
        {
            var sprite = new Sprite
            {
                UVs = new RectangleF(0.0f, 0.0f, 1.0f, 1.0f),
                Texture = texture,
                BorderRadius = 0,

                _borderElements = null
            };

            return sprite;
        }

        /// <summary>
        /// Base UVs of sprite.
        /// </summary>
        public RectangleF UVs { get; set; }

        /// <summary>
        /// Texture of sprite.
        /// </summary>
        public Texture2D Texture { get; set; }

        /// <summary>
        /// Size of border of sprite.
        /// </summary>
        public int BorderRadius { get; set; }
    }
}
