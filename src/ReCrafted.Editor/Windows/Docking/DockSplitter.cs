// ReCrafted Editor (c) 2016-2018 Always Too Late

using System.Diagnostics;
using System.Drawing;
using System.Numerics;

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

        /// <summary>
        /// Resizes this splitter to given size.
        /// </summary>
        /// <param name="size">The size percentage of this splitter. Must be in range [0.1-0.9].</param>
        internal void Divide(float size)
        {
            // Clamp
            if (size < 0.1f)
                size = 0.1f;
            if (size > 0.9f)
                size = 0.9f;

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

        /// <summary>
        /// Undocks given panel from this splitter. 
        /// Make sure that given panels is a level-0 child of this splitter. 
        /// </summary>
        /// <param name="panel">The children panel of this splitter to be undocked.</param>
        internal void Undock(DockPanelBase panel)
        {
            Debug.Assert(ChildA == panel || ChildB == panel);
            Debug.Assert(Parent is DockSplitter);

            var parentSplitter = (DockSplitter)Parent;

            if (ChildA == panel)
            {
                // Return ChildB to the Parent of this splitter
                if (parentSplitter.ChildA == this)
                    parentSplitter.ChildA = ChildB;
                else
                    parentSplitter.ChildB = ChildB;

                ChildB.Parent = parentSplitter;
            }

            if (ChildB == panel)
            {
                // Return ChildA to the Parent of this splitter
                if (parentSplitter.ChildB == this)
                    parentSplitter.ChildB = ChildA;
                else
                    parentSplitter.ChildA = ChildA;

                ChildA.Parent = parentSplitter;
            }
            
            MainWindow.Instance.DockPane.RecalculateLayout();
        }

        /// <summary>
        /// Checks if given position is inside this panel.
        /// </summary>
        /// <param name="pos">The position.</param>
        /// <returns>This object is returned when it is intersecting or null, otherwise.</returns>
        public override DockPanelBase FindIntersecting(Vector2 pos)
        {
            var a = ChildA?.FindIntersecting(pos);
            var b = ChildB?.FindIntersecting(pos);

            if (a != null)
            {
                if (a is DockSplitter)
                    return (a as DockSplitter).FindIntersecting(pos);

                return a;
            }

            if(b != null)
            {
                if (b is DockSplitter)
                    return (b as DockSplitter).FindIntersecting(pos);

                return b;
            }

            return null;
        }

        /// <summary>
        /// The size percentage of this splitter. Must be in range [0.1-0.9].
        /// </summary>
        public float Size { get; private set; }
        
        /// <summary>
        /// The type of docking in this splitter.
        /// </summary>
        public DockType DockType { get; internal set; }

        /// <summary>
        /// The first children in this splitter.
        /// </summary>
        public DockPanelBase ChildA { get; internal set; }

        /// <summary>
        /// The second children in this splitter. (Can be null!)
        /// </summary>
        public DockPanelBase ChildB { get; internal set; }
    }
}
