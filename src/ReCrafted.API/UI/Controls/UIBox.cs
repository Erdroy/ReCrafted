// ReCrafted © 2016-2017 Always Too Late

using ReCrafted.API.Graphics;
using ReCrafted.API.Mathematics;

namespace ReCrafted.API.UI.Controls
{
    /// <summary>
    /// UIBox control.
    /// </summary>
    public class UIBox : UIControl
    {
        /// <summary>
        /// Creates new UIBox.
        /// </summary>
        public UIBox()
        {
            _applyDefaults(new RectangleF(), null, Color.White);
        }

        /// <summary>
        /// Creates new UIBox.
        /// </summary>
        /// <param name="region">Region of new UIBox.</param>
        public UIBox(RectangleF region)
        {
            _applyDefaults(region, null, Color.White);
        }

        /// <summary>
        /// Creates new UIBox.
        /// </summary>
        /// <param name="color">Color of new UIBox.</param>
        public UIBox(Color color)
        {
            _applyDefaults(new RectangleF(), null, color);
        }

        /// <summary>
        /// Creates new UIBox.
        /// </summary>
        /// <param name="texture">Texture of new UIBox.</param>
        public UIBox(Texture2D texture)
        {
            _applyDefaults(new RectangleF(), texture, Color.White);
        }

        /// <summary>
        /// Creates new UIBox.
        /// </summary>
        /// <param name="region">Region of new UIBox.</param>
        /// <param name="texture">Texture of new UIBox.</param>
        public UIBox(RectangleF region, Texture2D texture)
        {
            _applyDefaults(region, texture, Color.White);
        }

        /// <summary>
        /// Creates new UIBox.
        /// </summary>
        /// <param name="region">Region of new UIBox.</param>
        /// <param name="color">Color of new UIColor.</param>
        public UIBox(RectangleF region, Color color)
        {
            _applyDefaults(region, null, color);
        }

        /// <summary>
        /// Creates new UIBox.
        /// </summary>
        /// <param name="region">Region of new UIBox.</param>
        /// <param name="texture">Texture of new UIBox.</param>
        /// <param name="color">Color of new UIBox.</param>
        public UIBox(RectangleF region, Texture2D texture, Color color)
        {
            _applyDefaults(region, texture, color);
        }

        // draw box
        public override void Draw()
        {
            if (!Enabled) return;
            UIInternal.Color = BoxColor;
            UIInternal.Depth = Depth;
            if (BoxTexture == null)
                UIInternal.DrawBox(Region);
            else
            {
                var region = Region;
                var uvs = new RectangleF(0.0f, 0.0f, 1.0f, 1.0f);
                UIInternal.DrawTexture2D(BoxTexture.NativePtr, ref region, ref uvs);
            }
        }

        // set default properties
        private void _applyDefaults(RectangleF region, Texture2D texture, Color color)
        {
            Region = region;
            BoxTexture = texture;
            BoxColor = color;
            Enabled = true;
            IgnoreMouseCollision = false;
            IsMouseOver = false;
            Parent = null;
        }

        /// <summary>
        /// Box texture.
        /// </summary>
        public Texture2D BoxTexture { get; set; }

        /// <summary>
        /// Box color.
        /// </summary>
        public Color BoxColor { get; set; }
    }
}
