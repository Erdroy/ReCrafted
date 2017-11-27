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
        private UIBox() { }

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

        /// <summary>
        /// Creates new UIBox.
        /// </summary>
        /// <returns>Our newly created UIBox control.</returns>
        public static UIBox Create()
        {
            return Create(new RectangleF(), null, Color.White);
        }

        /// <summary>
        /// Creates new UIBox.
        /// </summary>
        /// <param name="region">The UIBox region.</param>
        /// <returns>Our newly created UIBox control.</returns>
        public static UIBox Create(RectangleF region)
        {
            return Create(region, null, Color.White);
        }

        /// <summary>
        /// Creates new UIBox.
        /// </summary>
        /// <param name="color">Color of this box.</param>
        /// <returns>Our newly created UIBox control.</returns>
        public static UIBox Create(Color color)
        {
            return Create(new RectangleF(), null, color);
        }

        /// <summary>
        /// Creates new UIBox.
        /// </summary>
        ///  /// <param name="texture">Texture of this box.</param>
        /// <returns>Our newly created UIBox control.</returns>
        public static UIBox Create(Texture2D texture)
        {
            return Create(new RectangleF(), texture, Color.White);
        }

        /// <summary>
        /// Creates new UIBox.
        /// </summary>
        /// <param name="region">The UIBox region.</param>
        /// <param name="color">Color of this box.</param>
        /// <returns>Our newly created UIBox control.</returns>
        public static UIBox Create(RectangleF region, Color color)
        {
            return Create(region, null, color);
        }

        /// <summary>
        /// Creates new UIBox.
        /// </summary>
        /// <param name="region">The UIBox region.</param>
        ///  /// <param name="texture">Texture of this box.</param>
        /// <returns>Our newly created UIBox control.</returns>
        public static UIBox Create(RectangleF region, Texture2D texture)
        {
            return Create(region, texture, Color.White);
        }

        /// <summary>
        /// Creates new UIBox.
        /// </summary>
        /// <param name="region">The UIBox region.</param>
        /// <param name="texture">Texture of this box.</param>
        /// <param name="color">Color of this box.</param>
        /// <returns>Our newly created UIBox control.</returns>
        public static UIBox Create(RectangleF region, Texture2D texture, Color color)
        {
            return new UIBox
            {
                Region = region,
                BoxTexture = texture,
                BoxColor = color,
                Enabled = true,
                IgnoreMouseCollision = false,
                IsMouseOver = false,
                Parent = null
            };
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
