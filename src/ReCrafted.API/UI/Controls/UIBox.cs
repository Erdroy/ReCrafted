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
    }
}
