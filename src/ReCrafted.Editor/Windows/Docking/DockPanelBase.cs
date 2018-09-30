// ReCrafted Editor (c) 2016-2018 Always Too Late

using System;
using System.Diagnostics;
using System.Drawing;
using System.Numerics;

namespace ReCrafted.Editor.Windows.Docking
{
    public class DockPanelBase : IDockHandler, IDockable
    {
        public void Resize(Rectangle rect)
        {
            Rect = rect;
            OnResize();
        }

        protected virtual void OnResize() { }
        protected internal virtual void OnDock() { }
        protected virtual void OnUndock() { }

        /// <summary>
        /// Docks given panel to this panel using specified docking type, size and direction.
        /// </summary>
        /// <param name="other">The panel that will be docked.</param>
        /// <param name="dockType">The dock type.</param>
        /// <param name="dockDirection">The dock direction.</param>
        /// <param name="sizeMul">The size percentage multiplier. Must be in range [0.1-0.9].</param>
        /// <returns>The docked panel (other).</returns>
        public virtual DockPanelBase Dock(DockPanelBase other, DockType dockType, DockDirection dockDirection, float sizeMul = 0.5f)
        {
            Debug.Assert(dockDirection != DockDirection.None);
            Debug.Assert(other.CanBeDocked);

            if (!(this is DockSplitter)) // TODO: Refactor
            {
                Debug.Assert(Parent is DockSplitter);
                var parentSplitter = (DockSplitter) Parent;

                if (parentSplitter.ChildA == this)
                {
                    var p1 = dockDirection == DockDirection.Left ? other : parentSplitter.ChildA;
                    var p2 = dockDirection == DockDirection.Left ? parentSplitter.ChildA : other;

                    var splitter = new DockSplitter(parentSplitter, p1, p2, dockType, sizeMul);
                    parentSplitter.ChildA = splitter;
                }
                else
                {
                    var p1 = dockDirection == DockDirection.Left ? other : parentSplitter.ChildB;
                    var p2 = dockDirection == DockDirection.Left ? parentSplitter.ChildB : other;

                    var splitter = new DockSplitter(parentSplitter, p1, p2, dockType, sizeMul);
                    parentSplitter.ChildB = splitter;
                }

                other.OnDock();
                return other;
            }
            else
            {
                var splitter = (DockSplitter) this;

                other.Parent = this;

                switch (splitter.DockType)
                {
                    case DockType.Fill:
                        splitter.DockType = dockType;
                        if (dockDirection == DockDirection.Up)
                        {
                            splitter.ChildB = splitter.ChildA;
                            splitter.ChildA = other;
                        }
                        else
                        {
                            splitter.ChildB = other;
                        }
                        break;
                    case DockType.Horizontal:
                    case DockType.Vertical:
                        if (dockDirection == DockDirection.Left)
                        {
                            splitter.ChildA.Dock(other, dockType, dockDirection, sizeMul);
                        }
                        else
                        {
                            splitter.ChildB.Dock(other, dockType, dockDirection, sizeMul);
                        }
                        break;
                    default:
                        throw new ArgumentOutOfRangeException();
                }

                other.OnDock();
                return other;
            }
        }

        /// <summary>
        /// Undocks this panel from panel. Possibly makes the window floating.
        /// </summary>
        public void Undock()
        {
            Debug.Assert(!(this is DockSplitter));
            Debug.Assert(Parent is DockSplitter);

            var splitter = (DockSplitter)Parent;
            splitter.Undock(this);
            OnUndock();
            Parent = null;
        }

        /// <summary>
        /// Checks if given position is inside this panel.
        /// </summary>
        /// <param name="pos">The position.</param>
        /// <returns>This object is returned when it is intersecting or null, otherwise.</returns>
        public virtual DockPanelBase FindIntersecting(Vector2 pos)
        {
            return Rect.IntersectsWith(new Rectangle((int)pos.X, (int)pos.Y, 1, 1)) ? this : null;
        }

        /// <summary>
        /// The size of this panel. Also the position and size of window.
        /// </summary>
        public Rectangle Rect { get; set; }
 
        /// <summary>
        /// The parent panel.
        /// </summary>
        public DockPanelBase Parent { get; set; }

        /// <summary>
        /// The state of this panel that makes it unable to dock when false.
        /// </summary>
        public virtual bool CanBeDocked { get; set; } = true;
    }
}
