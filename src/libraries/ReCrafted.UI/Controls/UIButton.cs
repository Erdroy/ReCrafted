// ReCrafted (c) 2016-2018 Always Too Late

using ReCrafted.UI.Math;

namespace ReCrafted.UI.Controls
{
    public class UIButton : UIControl
    {
        public override void RebuildLayout()
        {

        }

        public override void RebuildVertexBuffer(UIBuffer buffer)
        {
            buffer.Clear();
            buffer.MakeRectangle(Transform.Position, Transform.Size, Color.DarkGray);
        }
    }
}
