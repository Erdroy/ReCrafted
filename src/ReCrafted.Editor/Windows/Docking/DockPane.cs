// ReCrafted Editor (c) 2016-2018 Always Too Late

using System.Diagnostics;
using System.Drawing;

namespace ReCrafted.Editor.Windows.Docking
{
    public sealed class DockPane : DockPanelBase
    {
        public new void Resize(Rectangle rect)
        {
            Rect = rect;
            Root?.Resize(rect);
        }

        public override void DebugDraw()
        {
            Root.DebugDraw();
        }

        public override DockPanelBase Dock(DockPanelBase other, DockType dockType, DockDirection dockDirection, float sizeMul = 0.5f)
        {
            var splitter = Root as DockSplitter;

            switch (dockType)
            {
                case DockType.Fill:
                    Debug.Assert(Root == null);
                    Root = new DockSplitter(this, other);
                    break;
                case DockType.Horizontal:
                    if (splitter != null)
                    {
                        
                    }
                    break;
                case DockType.Vertical:
                    break;
            }

            return Root;
        }
        
        public DockPanelBase Root { get; private set; }
    }
}
