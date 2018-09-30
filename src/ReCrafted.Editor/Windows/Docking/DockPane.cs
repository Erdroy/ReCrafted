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
        
        public DockPanelBase Dock(DockPanelBase other)
        {
            // Dock to fill
            Debug.Assert(Root == null);
            Debug.Assert(other.CanBeDocked);
            Root = new DockSplitter(this, other);
            other.OnDock();
            return other;
        }

        public override DockPanelBase Dock(DockPanelBase other, DockType dockType, DockDirection dockDirection, float sizeMul = 0.5f)
        {
            // TODO: Docking implementation
            Debug.Assert(other.CanBeDocked);
            Debug.Assert(false);
            return null;
        }

        public void RecalculateLayout()
        {
            Root.Resize(Rect);
        }

        public DockPanelBase Root { get; private set; }
    }
}
