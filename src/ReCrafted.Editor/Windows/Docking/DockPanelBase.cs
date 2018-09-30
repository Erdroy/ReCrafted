// ReCrafted Editor (c) 2016-2018 Always Too Late

using System;
using System.Diagnostics;
using System.Drawing;

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
        
        public virtual DockPanelBase Dock(DockPanelBase other, DockType dockType, DockDirection dockDirection, float sizeMul = 0.5f)
        {
            Debug.Assert(dockDirection != DockDirection.None);

            if (!(this is DockSplitter))
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

                return other;
            }
        }

        public Rectangle Rect { get; set; }
        
        public DockPanelBase Parent { get; set; }
    }
}
