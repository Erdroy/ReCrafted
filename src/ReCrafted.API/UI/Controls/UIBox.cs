// ReCrafted © 2016-2017 Always Too Late

using ReCrafted.API.Mathematics;

namespace ReCrafted.API.UI.Controls
{
    /// <summary>
    /// UIBox control.
    /// </summary>
    public class UIBox : UIPanel
    {
        public new static UIBox Create(RectangleF region, UILayoutType layoutType)
        {
            // construct new panel
            var box = new UIBox
            {
                Enabled = true,
                Parent = null,
                Region = region,
                PanelColor = Color.White,
                Layout = new UILayout
                {
                    Type = layoutType,
                    Enabled = true,
                    Region = region,
                    PreferredSize = new Vector2(region.Width, region.Height),
                    ForceExpandHeigth = false,
                    ForceExpandWidth = false
                }
            };

            // set panel layout parent
            box.Layout.Parent = box;

            return box;
        }

        public override void Draw()
        {
            UIInternal.Color = BoxColor;
            UIInternal.DrawBox(Region);

            base.Draw();
        }

        public Color BoxColor { get; set; }
    }
}
