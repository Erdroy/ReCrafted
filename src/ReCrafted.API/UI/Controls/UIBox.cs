// ReCrafted © 2016-2017 Always Too Late

using ReCrafted.API.Mathematics;

namespace ReCrafted.API.UI.Controls
{
    /// <summary>
    /// UIBox control.
    /// </summary>
    public class UIBox : UIControl
    {
        public override void Draw()
        {
            UIInternal.Color = BoxColor;
            UIInternal.DrawBox(Region);
        }

        public Color BoxColor { get; set; }

        private UIBox() { }

        /// <summary>
        /// Creates new UIBox.
        /// </summary>
        /// <returns>Our newly created UIBox control.</returns>
        public static UIBox Create()
        {
            return Create(new RectangleF(), Color.White);
        }

        /// <summary>
        /// Creates new UIBox.
        /// </summary>
        /// <param name="region">The UIBox region.</param>
        /// <returns>Our newly created UIBox control.</returns>
        public static UIBox Create(RectangleF region)
        {
            return Create(region, Color.White);
        }

        /// <summary>
        /// Creates new UIBox.
        /// </summary>
        /// <param name="region">The UIBox region.</param>
        /// <param name="color">Color of this box.</param>
        /// <returns>Our newly created UIBox control.</returns>
        public static UIBox Create(RectangleF region, Color color)
        {
            return new UIBox
            {
                Region = region,
                BoxColor = color,
                Enabled = true,
                IgnoreMouseCollision = false,
                IsMouseOver = false,
                Parent = null
            };
        }
    }
}
