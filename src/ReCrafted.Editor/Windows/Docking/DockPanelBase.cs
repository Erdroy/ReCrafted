// ReCrafted Editor (c) 2016-2018 Always Too Late

using System;
using System.Diagnostics;
using System.Drawing;
using System.Numerics;

namespace ReCrafted.Editor.Windows.Docking
{
    public class DockPanelBase : IDockHandler
    {
        public void Resize(Rectangle rect)
        {
            Rect = rect;
            OnResize();
        }

        protected virtual void OnResize() { }

        public virtual void DebugDraw()
        {
            var drawList = ImGuiNET.ImGui.GetOverlayDrawList();
            drawList.AddRect(new Vector2(Rect.X, Rect.Y), new Vector2(Rect.X + Rect.Width, Rect.Y + Rect.Height), 0xFFFFFFFF, 0.0f, -1, 1.0f);
        }

        public virtual DockPanelBase Dock(DockPanelBase other, DockType dockType, DockDirection dockDirection, float sizeMul = 0.5f)
        {
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
