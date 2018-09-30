// ReCrafted Editor (c) 2016-2018 Always Too Late

using System.Diagnostics;
using System.Drawing;
using System.Numerics;

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
        
        /// <summary>
        /// Checks if given position is inside this panel.
        /// </summary>
        /// <param name="pos">The position.</param>
        /// <returns>This object is returned when it is intersecting or null, otherwise.</returns>
        public override DockPanelBase FindIntersecting(Vector2 pos)
        {
            return Root.FindIntersecting(pos);
        }

        public DockPanelBase Root { get; private set; }
    }
}
