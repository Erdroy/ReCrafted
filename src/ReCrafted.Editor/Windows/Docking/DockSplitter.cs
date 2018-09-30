// ReCrafted Editor (c) 2016-2018 Always Too Late

using System.Drawing;

namespace ReCrafted.Editor.Windows.Docking
{
    public class DockSplitter : DockPanelBase
    {
        public DockSplitter(DockPanelBase parent, DockPanelBase a, float size = 0.5f)
        {
            Parent = parent;
            ChildA = a;
            DockType = DockType.Fill;
            Size = size;

            ChildA.Parent = this;

            Divide(size);
        }

        public DockSplitter(DockPanelBase parent, DockPanelBase a, DockPanelBase b, DockType dockType, float size = 0.5f)
        {
            Parent = parent;
            ChildA = a;
            ChildB = b;
            DockType = dockType;
            Size = size;

            ChildA.Parent = this;
            ChildB.Parent = this;

            Divide(size);
        }

        protected override void OnResize()
        {
            Divide(Size);
        }

        public void Divide(float size)
        {
            var targetRect = Rect;

            switch (DockType)
            {
                case DockType.Fill:
                {
                    ChildA.Resize(targetRect);
                    break;
                }
                case DockType.Horizontal:
                {
                    var aHeight = (int)(targetRect.Height * size);
                    var bHeight = targetRect.Height - aHeight;

                    var rectA = new Rectangle(targetRect.X, targetRect.Y, targetRect.Width, aHeight);
                    var rectB = new Rectangle(targetRect.X, targetRect.Y + aHeight, targetRect.Width, bHeight);

                    ChildA.Resize(rectA);
                    ChildB.Resize(rectB);
                    break;
                }
                case DockType.Vertical:
                {
                    var aWidth = (int)(targetRect.Width * size);
                    var bWidth = targetRect.Width - aWidth;

                    var rectA = new Rectangle(targetRect.X, targetRect.Y, aWidth, targetRect.Height);
                    var rectB = new Rectangle(targetRect.X + aWidth, targetRect.Y, bWidth, targetRect.Height);

                    ChildA.Resize(rectA);
                    ChildB.Resize(rectB);
                    break;
                }
            }

            Size = size;
        }

        public float Size { get; private set; }
        public DockType DockType { get; internal set; }
        public DockPanelBase ChildA { get; internal set; }
        public DockPanelBase ChildB { get; internal set; }
    }
}
