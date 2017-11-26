// ReCrafted © 2016-2017 Always Too Late

using ReCrafted.API.Common;
using ReCrafted.API.Core;
using ReCrafted.API.Mathematics;

namespace ReCrafted.API.UI.Controls
{
    /// <summary>
    /// Delegate for UIButton click event.
    /// </summary>
    public delegate void ButtonClick();

    /// <summary>
    /// UIButton control.
    /// </summary>
    public class UIButton : UIControl
    {
        //current button color
        private Color _color;

        /// <summary>
        /// Delegate will be invoked, when button has been clicked.
        /// </summary>
        public ButtonClick OnClick;

        public override void OnMouseEnter()
        {
            if (!SmoothColors)
                _color = Colors.OverColor;
        }

        public override void OnMouseOver()
        {
            if (SmoothColors)
            {
                if (Input.IsKeyUp(Keys.Mouse0))
                    _color = Colors.ClickColor;
            }
            else
            {
                _color = Input.IsKey(Keys.Mouse0) ? Colors.ClickColor : Colors.OverColor;
            }

            if (Input.IsKeyUp(Keys.Mouse0))
                OnClick?.Invoke();
        }

        public override void OnMouseExit()
        {
            if (!SmoothColors)
                _color = Colors.NormalColor;
        }

        public override void Draw()
        {
            if (SmoothColors) _color = Color.Lerp(_color, IsMouseOver ? Colors.OverColor : Colors.NormalColor, (float)Time.DeltaTime * SmoothTranslation);
            UIInternal.Color = _color;
            UIInternal.DrawBox(Region);
        }

        private UIButton() { }

        /// <summary>
        /// Creates new UIButton.
        /// </summary>
        /// <returns>Our newly created UIButton control.</returns>
        public static UIButton Create()
        {
            return Create(new RectangleF(), UIButtonColors.Defaults);
        }

        /// <summary>
        /// Creates new UIButton.
        /// </summary>
        /// <param name="colors">Colors of the UIButton.</param>
        /// <returns>Our newly created UIButton control.</returns>
        public static UIButton Create(UIButtonColors colors)
        {
            return Create(new RectangleF(), colors);
        }

        /// <summary>
        /// Creates new UIButton.
        /// </summary>
        /// <param name="region">The UIButton region.</param>
        /// <returns>Our newly created UIButton control.</returns>
        public static UIButton Create(RectangleF region)
        {
            return Create(region, UIButtonColors.Defaults);
        }

        /// <summary>
        /// Creates new UIButton.
        /// </summary>
        /// <param name="region">The UIButton region.</param>
        /// <param name="colors">Colors of the UIButton.</param>
        /// <returns>Our newly created UIButton control.</returns>
        public static UIButton Create(RectangleF region, UIButtonColors colors)
        {
            return new UIButton
            {
                Region = region,
                _color = Color.White, 
                Colors =  colors,
                SmoothColors = true,
                SmoothTranslation = 5f,
                Enabled = true,
                IgnoreMouseCollision = false,
                IsMouseOver = false,
                Parent = null
            };
        }

        /// <summary>
        /// Colors of button.
        /// </summary>
        public UIButtonColors Colors { get; set; }

        /// <summary>
        /// Colors on button will be changed smoothly.
        /// </summary>
        public bool SmoothColors { get; set; }

        /// <summary>
        /// Speed of smooth translation.
        /// </summary>
        public float SmoothTranslation { get; set; }
    }
}
