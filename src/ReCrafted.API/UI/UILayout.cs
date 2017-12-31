// ReCrafted © 2016-2017 Always Too Late

using System;
using System.Linq;
using ReCrafted.API.Common;
using ReCrafted.API.Mathematics;

namespace ReCrafted.API.UI
{
    /// <summary>
    /// UILayoutType enum.
    /// </summary>
    public enum UILayoutType
    {
        Grid,
        DynamicGrid,
        Horizontal,
        Vertical
    }

    /// <summary>
    /// UILayout alignment.
    /// </summary>
    public enum UILayoutAlignment
    {
        LeftTop,
        LeftMiddle,
        LeftBottom,
        MiddleTop,
        Middle,
        MiddleBottom,
        RightTop,
        RightMiddle,
        RightBottom
    }

    /// <summary>
    /// UILayout alignment posiiton.
    /// </summary>
    public enum UILayoutAlignmentPosition
    {
        Top,
        Middle,
        Bottom
    }

    /// <summary>
    /// UILayout alignment side.
    /// </summary>
    public enum UILayoutAlignmentSide
    {
        Left,
        Middle,
        Right,
    }

    /// <summary>
    /// UILayout class, implements lay-outing system.
    /// </summary>
    public class UILayout : UIContainer
    {
        // if region of this parent has been changed, recalculate current layout
        public override void OnRegionChanged()
        {
            Recalculate(Region);
        }

        // search for mouse collision in all childrens of this layout.
        internal override UIControl OnMouseCollision()
        {
            return LookForMouseCollision(false);
        }

        /// <summary>
        /// Gets position of current layout alignment.
        /// </summary>
        /// <returns></returns>
        public UILayoutAlignmentPosition GetLayoutAlignmentPosition()
        {
            var str = Alignment.ToString();
            if (str.EndsWith("Middle"))
                return UILayoutAlignmentPosition.Middle;
            return str.EndsWith("Bottom") ? UILayoutAlignmentPosition.Bottom : UILayoutAlignmentPosition.Top;
        }

        /// <summary>
        /// Gets side of current layout alignment.
        /// </summary>
        /// <returns></returns>
        public UILayoutAlignmentSide GetLayoutAlignmentSide()
        {
            var str = Alignment.ToString();
            if (str.StartsWith("Middle"))
                return UILayoutAlignmentSide.Middle;
            return str.StartsWith("Right") ? UILayoutAlignmentSide.Right : UILayoutAlignmentSide.Left;
        }

        /// <summary>
        /// Recalculates Layout, duh.
        /// </summary>
        /// <param name="parentRegion">The region in which the layout will be calculated.</param>
        /// <returns>New recalculated region of layout.</returns>
        public RectangleF Recalculate(RectangleF parentRegion)
        {
            return InternalRecalculate(parentRegion, false);
        }

        /// <summary>
        /// Pre-calculates Layout, duh.
        /// </summary>
        /// <param name="parentRegion">The region in which the layout will be calculated.</param>
        /// <returns>New recalculated region of layout.</returns>
        internal RectangleF Precalculate(RectangleF parentRegion)
        {
            return InternalRecalculate(parentRegion, true);
        }

        /// <summary>
        /// Recalculates Layout, duh.
        /// </summary>
        /// <param name="parentRegion">The region in which the layout will be calculated.</param>
        /// <param name="precalculate">Is this the pre calculation?</param>
        /// <returns>New recalculated region of layout.</returns>
        internal RectangleF InternalRecalculate(RectangleF parentRegion, bool precalculate)
        {
            Profiler.BeginProfile("UILayout.Recalculate");
            // apply current region of panel with padding to layout region.

            var newLayoutRegion = parentRegion;
            newLayoutRegion.X += Padding.Left;
            newLayoutRegion.Y += Padding.Top;
            newLayoutRegion.Width -= Padding.Right + Padding.Left;
            newLayoutRegion.Height -= Padding.Bottom + Padding.Top;
            Region = newLayoutRegion;

            // some calculations
            var verticalSize = Region.Height / Controls.Count;
            var horizontalSize = Region.Width / Controls.Count;

            var calculatedWidth = 0f;
            var calculatedHeight = 0f;

            var totalContentHeight = GetTotalContentHeight();
            var totalContentWidth = GetTotalContentWidth();

            var position = GetLayoutAlignmentPosition();
            var side = GetLayoutAlignmentSide();

            // apply layout
            for (var index = 0; index < Controls.Count; index++)
            {
                var currentControl = Controls[index];
                var previousControl = index == 0 ? null : Controls[index - 1];
                var newRegion = new RectangleF(0f, 0f, 0f, 0f)
                {
                    X = CalculateRegionXForControl(side, totalContentWidth, index, previousControl?.Region.X ?? 0,
                        previousControl?.Region.Width ?? 0, currentControl.Region.Width),
                    Y = CalculateRegionYForControl(position, totalContentHeight, index, previousControl?.Region.Y ?? 0,
                        previousControl?.Region.Height ?? 0, currentControl.Region.Height)
                };

                switch (Type)
                {
                    case UILayoutType.Grid:
                        throw new NotImplementedException("Grid layout is not implemented yet :/");
                    case UILayoutType.DynamicGrid:
                        throw new NotImplementedException("DynamicGrid layout is not implemented yet :/");
                    case UILayoutType.Horizontal:
                    {
                        #region APPLY_HORIZONTAL_SIZE

                        if (!currentControl.IgnoreLayoutResize)
                        {
                            if (ForceExpandWidth)
                            {
                                if (currentControl.PreferredSize.X > 0.01f)
                                {
                                    // apply preferred size of control
                                    newRegion.Width = currentControl.PreferredSize.X;

                                    calculatedWidth += newRegion.Width;

                                    // recalculate horizontal size for next controls
                                    horizontalSize = (Region.Width - calculatedWidth) / (Controls.Count - index);
                                }
                                else
                                {
                                    // apply size of horizontal force expand
                                    newRegion.Width = horizontalSize;

                                    calculatedWidth += newRegion.Width;
                                }

                                // apply horizontal space
                                newRegion.Width -= Space / 1.5f;
                            }
                            else
                            {
                                // apply default size of control
                                newRegion.Width = currentControl.Region.Width;
                                if (currentControl.PreferredSize.X > 0.01f &&
                                    newRegion.Width < currentControl.PreferredSize.X)
                                {
                                    // apply preferred size if current width is less than preferred value
                                    newRegion.Width = currentControl.PreferredSize.X;
                                }
                            }

                            if (ForceExpandHeight)
                            {
                                // apply height of current layout
                                newRegion.Height = Region.Height;
                            }
                            else
                            {
                                // apply default size of control
                                newRegion.Height = currentControl.Region.Height;
                                if (currentControl.PreferredSize.Y > 0.01f &&
                                    newRegion.Height < currentControl.PreferredSize.Y)
                                {
                                    // apply preferred size if current height is less than preferred value
                                    newRegion.Height = currentControl.PreferredSize.Y;
                                }
                            }
                        }

                        #endregion

                        break;
                    }
                    case UILayoutType.Vertical:
                    {
                        #region APPLY_VERTICAL_SIZE

                        if (!currentControl.IgnoreLayoutResize)
                        {
                            if (ForceExpandWidth)
                            {
                                // apply width of current layout
                                newRegion.Width = Region.Width;
                            }
                            else
                            {
                                // apply default size of control
                                newRegion.Width = currentControl.Region.Width;
                                if (currentControl.PreferredSize.X > 0.01f &&
                                    newRegion.Width < currentControl.PreferredSize.X)
                                {
                                    // apply preferred size if current height is less than preferred value
                                    newRegion.Width = currentControl.PreferredSize.X;
                                }
                            }

                            if (ForceExpandHeight)
                            {
                                if (currentControl.PreferredSize.Y > 0.01f)
                                {
                                    // apply preferred size of control
                                    newRegion.Height = currentControl.PreferredSize.Y;

                                    calculatedHeight += newRegion.Height;

                                    // recalculate vertical  size for next controls
                                    verticalSize = (Region.Height - calculatedHeight) / (Controls.Count - index);
                                }
                                else
                                {
                                    // apply size of horizontal force expand
                                    newRegion.Height = verticalSize;

                                    calculatedHeight += newRegion.Height;
                                }

                                // apply vertical space
                                newRegion.Height -= Space / 1.5f;
                            }
                            else
                            {
                                // apply default size of control
                                newRegion.Height = currentControl.Region.Height;
                                if (currentControl.PreferredSize.Y > 0.01f &&
                                    newRegion.Height < currentControl.PreferredSize.Y)
                                {
                                    // apply preferred size if current height is less than preferred value
                                    newRegion.Height = currentControl.PreferredSize.Y;
                                }
                            }
                        }

                        #endregion

                        break;
                    }
                    default:
                        throw new ArgumentOutOfRangeException();
                }

                if (!currentControl.Enabled && !precalculate)
                {
                    newRegion.Width = 0;
                    newRegion.Height = 0;
                }

                newRegion = new RectangleF(newRegion.X + Offset.X, newRegion.Y + Offset.Y, newRegion.Width,
                    newRegion.Height);
                var regionChanged = currentControl.Region != newRegion;
                currentControl.Region = newRegion;
                if (regionChanged)
                {
                    currentControl.OnRegionChanged();
                    OnControlsChanged?.Invoke();
                }
            }

            Profiler.EndProfile();
            return Region;
        }

        // get height of all controls in container
        private float GetTotalContentHeight()
        {
            return Controls.Sum(c => c.Region.Height + Space);
        }

        // get width of all controls in container
        private float GetTotalContentWidth()
        {
            return Controls.Sum(c => c.Region.Width + Space);
        }

        // calculates new region y of control based on current layout
        private float CalculateRegionYForControl(UILayoutAlignmentPosition position, float totalContentHeight,
            int controlIndex, float previousY, float previousHeight, float currentHeight)
        {
            switch (Type)
            {
                case UILayoutType.Grid:
                    throw new NotImplementedException();
                case UILayoutType.DynamicGrid:
                    throw new NotImplementedException();
                case UILayoutType.Horizontal:
                    switch (position)
                    {
                        case UILayoutAlignmentPosition.Top:
                            return Region.Y;
                        case UILayoutAlignmentPosition.Middle:
                            return Region.Y + Region.Height / 2 - currentHeight / 2;
                        case UILayoutAlignmentPosition.Bottom:
                            return Region.Y + Region.Height - currentHeight;
                        default:
                            throw new ArgumentOutOfRangeException();
                    }
                case UILayoutType.Vertical:
                    switch (position)
                    {
                        case UILayoutAlignmentPosition.Top:
                            return controlIndex == 0 ? Region.Y : previousY + previousHeight + Space;
                        case UILayoutAlignmentPosition.Middle:
                            return controlIndex == 0
                                ? Region.Y + Region.Height / 2 - totalContentHeight / 2
                                : previousY + previousHeight + Space;
                        case UILayoutAlignmentPosition.Bottom:
                            return controlIndex == 0
                                ? Region.Y + Region.Height - currentHeight
                                : previousY - currentHeight - Space;
                        default:
                            throw new ArgumentOutOfRangeException();
                    }
                default:
                    throw new ArgumentOutOfRangeException();
            }
        }

        // calculates new region x of control based on current layout
        private float CalculateRegionXForControl(UILayoutAlignmentSide side, float totalContentWidth, int controlIndex,
            float previousX, float previousWidth, float currentWidth)
        {
            switch (Type)
            {
                case UILayoutType.Grid:
                    throw new NotImplementedException();
                case UILayoutType.DynamicGrid:
                    throw new NotImplementedException();
                case UILayoutType.Horizontal:
                    switch (side)
                    {
                        case UILayoutAlignmentSide.Left:
                            return controlIndex == 0 ? Region.X : previousX + previousWidth + Space;
                        case UILayoutAlignmentSide.Middle:
                            return controlIndex == 0
                                ? Region.X + Region.Width / 2 - totalContentWidth / 2
                                : previousX + previousWidth + Space;
                        case UILayoutAlignmentSide.Right:
                            return controlIndex == 0 ? Region.X + Region.Width - currentWidth : previousX - currentWidth - Space;
                        default:
                            throw new ArgumentOutOfRangeException();
                    }
                case UILayoutType.Vertical:
                    switch (side)
                    {
                        case UILayoutAlignmentSide.Left:
                            return Region.X;
                        case UILayoutAlignmentSide.Middle:
                            return Region.X + Region.Width / 2 - currentWidth / 2;
                        case UILayoutAlignmentSide.Right:
                            return Region.X + Region.Width - currentWidth;
                        default:
                            throw new ArgumentOutOfRangeException();
                    }
                default:
                    throw new ArgumentOutOfRangeException();
            }
        }

        /// <summary>
        /// Creates new UILayout.
        /// </summary>
        /// <param name="region">The UILayout region.</param>
        /// <param name="layoutType">The lay-outing type.</param>
        /// <param name="layoutAlignment">Alignment type.</param>
        /// <returns>The newly created layout.</returns>
        public static UILayout Create(RectangleF region, UILayoutType layoutType = UILayoutType.Vertical,
            UILayoutAlignment layoutAlignment = UILayoutAlignment.LeftTop)
        {
            // construct new panel
            var layout = new UILayout
            {
                Enabled = true,
                Parent = null,
                Region = region,
                Type = layoutType,
                PreferredSize = new Vector2(region.Width, region.Height),
                ForceExpandHeight = false,
                ForceExpandWidth = false,
                Padding = new UIPadding(),
                Space = 0,
                Alignment = layoutAlignment
            };

            return layout;
        }

        /// <summary>
        /// The layout type.
        /// </summary>
        public UILayoutType Type { get; set; }

        /// <summary>
        /// Offset of layout content.
        /// </summary>
        public Vector2 Offset { get; set; }

        /// <summary>
        /// Forces width of all children to the width of this layout.
        /// </summary>
        public bool ForceExpandWidth { get; set; }

        /// <summary>
        /// Forces height of all children.
        /// </summary>
        public bool ForceExpandHeight { get; set; }

        /// <summary>
        /// Layout padding.
        /// </summary>
        public UIPadding Padding { get; set; }

        /// <summary>
        /// Space between controls.
        /// </summary>
        public float Space { get; set; }

        /// <summary>
        /// Layout alignment.
        /// </summary>
        public UILayoutAlignment Alignment { get; set; }
    }
}
