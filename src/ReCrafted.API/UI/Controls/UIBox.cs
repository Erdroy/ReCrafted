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
        /// <param name="sprite">Sprite of new UIBox.</param>
        public UIBox(Sprite sprite)
        {
            _applyDefaults(new RectangleF(), sprite, Color.White);
        }

        /// <summary>
        /// Creates new UIBox.
        /// </summary>
        /// <param name="region">Region of new UIBox.</param>
        /// <param name="sprite">Sprite of new UIBox.</param>
        public UIBox(RectangleF region, Sprite sprite)
        {
            _applyDefaults(region, sprite, Color.White);
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
        /// <param name="sprite">Sprite of new UIBox.</param>
        /// <param name="color">Color of new UIBox.</param>
        public UIBox(RectangleF region, Sprite sprite, Color color)
        {
            _applyDefaults(region, sprite, color);
        }

        // draw box
        public override void Draw()
        {
            if (!Enabled) return;
            UIInternal.Color = BoxColor;
            UIInternal.Depth = Depth;
            if (Sprite == null)
                UIInternal.DrawBox(Region);
            else
            {
                Sprite.Draw(Region);
            }
        }

        // reset box
        public override void Reset()
        {
            
        }

        // set default properties
        private void _applyDefaults(RectangleF region, Sprite sprite, Color color)
        {
            Region = region;
            Sprite = sprite;
            BoxColor = color;
            Enabled = true;
            IgnoreMouseCollision = false;
            IsMouseOver = false;
            Parent = null;
        }

        /// <summary>
        /// Box texture.
        /// </summary>
        public Sprite Sprite { get; set; }

        /// <summary>
        /// Box color.
        /// </summary>
        public Color BoxColor { get; set; }
    }
}
