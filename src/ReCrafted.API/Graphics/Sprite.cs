// ReCrafted © 2016-2017 Always Too Late

using System;
using System.Collections.Generic;
using ReCrafted.API.Core;
using ReCrafted.API.Mathematics;
using ReCrafted.API.UI;

namespace ReCrafted.API.Graphics
{
    /// <summary>
    /// Sprite class.
    /// </summary>
    public class Sprite
    {
        private static readonly List<Sprite> Sprites = new List<Sprite>();

        // region of sprite
        private RectangleF _region;

        // last calculated region
        private int _lastCalculatedRadius;
        // last calculated uvs
        private RectangleF _lastCalculatedUVs;

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

        private Sprite()
        {
            _borderElements = null;
        }

        /// <summary>
        /// Destroys this sprite.
        /// </summary>
        public void Destroy()
        {
            Sprites.Remove(this);
        }

        // draw
        internal void Draw()
        {
            UIInternal.Depth = Depth;
            UIInternal.Color = Color;
            if (_borderElements == null)
            {
                var region = _region;
                if (Texture != null)
                {
                    var uv = UVs;
                    UIInternal.DrawTexture2D(Texture.NativePtr, ref region, ref uv);
                }
                else
                {
                    UIInternal.DrawBox(region);
                }
            }
            else
            {
                foreach (var b in _borderElements)
                {
                    var uv = b.UVs;
                    var region = b.Region;
                    UIInternal.DrawTexture2D(Texture.NativePtr, ref region, ref uv);
                }
            }
        }

        // calculate 9-slice
        private void CalculateBorders(int radius, RectangleF uvs)
        {
            _borderElements = new BorderElement[9];
            _lastCalculatedRadius = radius;
            _lastCalculatedUVs = uvs;


            var rw = radius / (float) Texture.Width;
            var rh = radius / (float) Texture.Height;
            // x--
            // ---
            // ---
            _borderElements[0].UVs = new RectangleF(0.0f, 0.0f, rw, rh);
            _borderElements[0].Region = new RectangleF(_region.X, _region.Y, radius, radius);

            // -x-
            // ---
            // ---
            _borderElements[1].UVs = new RectangleF(rw, 0.0f, 1f - rw * 3f, rh);
            _borderElements[1].Region = new RectangleF(_region.X + radius, _region.Y, _region.Width - radius * 2, radius);

            // --x
            // ---
            // ---
            _borderElements[2].UVs = new RectangleF(1f - rw * 3.5f, 0.0f, rw, rh);
            _borderElements[2].Region = new RectangleF(_region.X + _region.Width - radius, _region.Y, radius, radius);

            // ---
            // x--
            // ---
            _borderElements[3].UVs = new RectangleF(0, rh, rw, 1f - rh * 3f);
            _borderElements[3].Region = new RectangleF(_region.X, _region.Y + radius, radius, _region.Height - radius * 2f);

            // ---
            // -x-
            // ---
            _borderElements[4].UVs = new RectangleF(rw, rh, 1f - rw * 3f, 1f - rh * 3f);
            _borderElements[4].Region = new RectangleF(_region.X + radius, _region.Y + radius, _region.Width - radius * 2f, _region.Height - radius * 2f);

            // ---
            // --x
            // ---
            _borderElements[5].UVs = new RectangleF(1f - rw * 3.5f, rh, rw, 1f - rh * 3f);
            _borderElements[5].Region = new RectangleF(_region.X + _region.Width - radius, _region.Y + radius, radius, _region.Height - radius * 2f);

            // ---
            // ---
            // x--
            _borderElements[6].UVs = new RectangleF(0.0f, 1f - rh * 3.5f, rw, rh);
            _borderElements[6].Region = new RectangleF(_region.X, _region.Y + _region.Height - radius, radius, radius);

            // ---
            // ---
            // -x-
            _borderElements[7].UVs = new RectangleF(rw, 1f - rh * 3.5f, 1f - rw * 3f, rh);
            _borderElements[7].Region = new RectangleF(_region.X + radius, _region.Y + +_region.Height - radius, _region.Width - radius * 2, radius);

            // ---
            // ---
            // --x
            _borderElements[8].UVs = new RectangleF(1f - rw * 3.5f, 1f - rh * 3.5f, rw, rh);
            _borderElements[8].Region = new RectangleF(_region.X + _region.Width - radius, _region.Y + +_region.Height - radius, radius, radius);
        }

        // draws all sprites
        internal static void DrawAll()
        {
            for (var index = Sprites.Count - 1; index >= 0; index--)
            {
                var sprite = Sprites[index];
                try
                {
                    sprite.Draw();
                }
                catch (Exception ex)
                {
                    Logger.Write(ex.ToString(), LogLevel.Error);
                }
            }
        }

        /// <summary>
        /// Creates new sprite instance.
        /// </summary>
        /// <param name="region">Region of sprite.</param>
        /// <param name="texture">Texture of sprite.</param>
        /// <param name="baseDepth">Base depth of sprite.</param>
        /// <returns></returns>
        public static Sprite Create(RectangleF region, Texture2D texture, int baseDepth = 1000)
        {
            var sprite = new Sprite
            {
                _region = region,
                UVs = new RectangleF(0.0f, 0.0f, 1.0f, 1.0f),
                Texture = texture,
                Color = Color.White,
                Depth = baseDepth
            };
            Sprites.Add(sprite);
            return sprite;
        }

        /// <summary>
        /// Creates new bordered sprite.
        /// </summary>
        /// <param name="region">Region fo sprite.</param>
        /// <param name="texture">Texture of sprite.</param>
        /// <param name="borderRadius">Border radius.</param>
        /// <param name="baseDepth">Base depth of sprite.</param>
        /// <returns></returns>
        public static Sprite CreateBordered(RectangleF region, Texture2D texture, int borderRadius = 10, int baseDepth = 1000)
        {
            var sprite = Create(region, texture, baseDepth);
            sprite.CalculateBorders(borderRadius, new RectangleF(0f, 0f, 1f, 1f));
            return sprite;
        }

        /// <summary>
        /// Creates new bordered sprite.
        /// </summary>
        /// <param name="region">Region fo sprite.</param>
        /// <param name="texture">Texture of sprite.</param>
        /// <param name="uvs">Base UVs of bordered sprite.</param>
        /// <param name="borderRadius">Border radius.</param>
        /// <param name="baseDepth">Base depth of sprite.</param>
        /// <returns></returns>
        public static Sprite CreateBordered(RectangleF region, Texture2D texture, RectangleF uvs, int borderRadius = 10, int baseDepth = 1000)
        {
            var sprite = Create(region, texture, baseDepth);
            sprite.CalculateBorders(borderRadius, uvs);
            return sprite;
        }

        /// <summary>
        /// UVs of sprite.
        /// </summary>
        public RectangleF UVs { get; set; }

        /// <summary>
        /// Texture of sprite.
        /// </summary>
        public Texture2D Texture { get; set; }

        /// <summary>
        /// Color of sprite.
        /// </summary>
        public Color Color { get; set; }

        /// <summary>
        /// Depth of sprite.
        /// </summary>
        public int Depth { get; set; }

        /// <summary>
        /// Position of sprite.
        /// </summary>
        public Vector2 Position
        {
            get { return new Vector2(_region.X, _region.Y); }
            set { _region = new RectangleF(value.X, value.Y, _region.Width, _region.Height);
                if (_borderElements != null) CalculateBorders(_lastCalculatedRadius, _lastCalculatedUVs);
            }
        }

        /// <summary>
        /// Size of sprite.
        /// </summary>
        public Vector2 Size
        {
            get { return new Vector2(_region.Width, _region.Height); }
            set { _region = new RectangleF(_region.X, _region.Y, value.X, value.Y);
                if (_borderElements != null) CalculateBorders(_lastCalculatedRadius, _lastCalculatedUVs);               
            }
        }
    }
}
