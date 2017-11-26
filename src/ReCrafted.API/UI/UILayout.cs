// ReCrafted © 2016-2017 Always Too Late

using System;
using ReCrafted.API.Core;
using ReCrafted.API.Mathematics;

namespace ReCrafted.API.UI
{
    /// <summary>
    /// UILayoutType enum
    /// </summary>
    public enum UILayoutType
    {
        Grid,
        DynamicGrid,
        Horizontal,
        Vertical
    }

    /// <summary>
    /// UILayout class, implements lay-outing system.
    /// </summary>
    public class UILayout : UIContainer
    {
        /// <summary>
        /// Recalculates Layout, duh.
        /// </summary>
        /// <param name="parentRegion">The region in which the layout will be calculated.</param>
        public void Recalculate(RectangleF parentRegion)
        {
            //Apply current region of panel with padding to layout region.
            var newLayoutRegion = parentRegion;
            newLayoutRegion.X += Padding.Right;
            newLayoutRegion.Y += Padding.Top;
            newLayoutRegion.Width -= Padding.Right + Padding.Left;
            newLayoutRegion.Height -= Padding.Bottom + Padding.Top;
            Region = newLayoutRegion;

            var veticalSize = Region.Height / Controls.Count;
            var horizontalSize = Region.Width / Controls.Count;

            for (var index = 0; index < Controls.Count; index++)
            {
                var currentControl = Controls[index];
                switch (Type)
                {
                    case UILayoutType.Grid:
                        throw new NotImplementedException("Grid layout is not implemented yet :/");
                    case UILayoutType.DynamicGrid:
                        throw new NotImplementedException("DynamicGrid layout is not implemented yet :/");
                    case UILayoutType.Horizontal:
                    {
                        var previousControl = index == 0 ? null : Controls[index - 1];
                        var newRegion = new RectangleF(0f, 0f, 0f, 0f)
                        {
                            X = Region.X,
                            Y = Region.Y
                        };
                        if (previousControl != null)
                            newRegion.X = previousControl.Region.X + previousControl.Region.Width + Space;

                        if (ForceExpandWidth)
                        {
                            newRegion.Width = horizontalSize;
                            if (Math.Abs(PreferredSize.X) > 0.01f)
                            {
                                if (newRegion.Width < PreferredSize.X)
                                    newRegion.Width = PreferredSize.X;
                            }
                            newRegion.Width -= Space / 1.5f;
                        }
                        else
                        {
                            newRegion.Width = currentControl.Region.Width;
                            if (Math.Abs(PreferredSize.X) > 0.01f)
                            {
                                if (newRegion.Width < PreferredSize.X)
                                    newRegion.Width = PreferredSize.X;
                            }
                        }

                        if (ForceExpandHeigth)
                        {
                            newRegion.Height = Region.Height;
                            if (Math.Abs(PreferredSize.Y) > 0.01f)
                            {
                                if (newRegion.Height < PreferredSize.Y)
                                    newRegion.Height = PreferredSize.Y;
                            }
                        }
                        else
                        {
                            newRegion.Height = currentControl.Region.Height;
                            if (Math.Abs(PreferredSize.Y) > 0.01f)
                            {
                                if (newRegion.Height < PreferredSize.Y)
                                    newRegion.Height = PreferredSize.Y;
                            }
                        }
                        var regionChanged = currentControl.Region != newRegion;
                        currentControl.Region = newRegion;
                        if (regionChanged)
                            currentControl.OnRegionChanged();
                        break;
                    }
                    case UILayoutType.Vertical:
                    {
                        var previousControl = index == 0 ? null : Controls[index - 1];
                        var newRegion = new RectangleF(0f, 0f, 0f, 0f)
                        {
                            X = Region.X,
                            Y = Region.Y
                        };
                        if (previousControl != null)
                            newRegion.Y = previousControl.Region.Y + previousControl.Region.Height + Space;

                        if (ForceExpandWidth)
                        {
                            newRegion.Width = Region.Width;
                            if (Math.Abs(PreferredSize.X) > 0.01f)
                            {
                                if (newRegion.Width < PreferredSize.X)
                                    newRegion.Width = PreferredSize.X;
                            }
                        }
                        else
                        {
                            newRegion.Width = currentControl.Region.Width;
                            if (Math.Abs(PreferredSize.X) > 0.01f)
                            {
                                if (newRegion.Width < PreferredSize.X)
                                    newRegion.Width = PreferredSize.X;
                            }
                        }

                        if (ForceExpandHeigth)
                        {
                            newRegion.Height = veticalSize;
                            if (Math.Abs(PreferredSize.Y) > 0.01f)
                            {
                                if (newRegion.Height < PreferredSize.Y)
                                    newRegion.Height = PreferredSize.Y;
                            }

                            newRegion.Height -= Space / 1.5f;
                        }
                        else
                        {
                            newRegion.Height = currentControl.Region.Height;
                            if (Math.Abs(PreferredSize.Y) > 0.01f)
                            {
                                if (newRegion.Height < PreferredSize.Y)
                                    newRegion.Height = PreferredSize.Y;
                            }
                        }

                        var regionChanged = currentControl.Region != newRegion;
                        currentControl.Region = newRegion;
                        if (regionChanged)
                                currentControl.OnRegionChanged();
                        break;
                    }
                    default:
                        throw new ArgumentOutOfRangeException();
                }
            }
        }

        /// <summary>
        /// The layout type.
        /// </summary>
        public UILayoutType Type { get; set; }

        /// <summary>
        /// Preferred size for Grid and H/V layout type.
        /// </summary>
        public Vector2 PreferredSize { get; set; }

        /// <summary>
        /// Forces width of all childrens to the width of this layout.
        /// </summary>
        public bool ForceExpandWidth { get; set; }

        /// <summary>
        /// Forces height of all childrens.
        /// </summary>
        public bool ForceExpandHeigth { get; set; }

        /// <summary>
        /// Layout padding.
        /// </summary>
        public UIPadding Padding { get; set; }

        /// <summary>
        /// Space between controls.
        /// </summary>
        public float Space { get; set; }

        public override void OnRegionChanged()
        {
            Recalculate(Region);
        }

        public override bool OnMouseCollision()
        {
            return LookForMouseCollision();
        }

        /// <summary>
        /// Creates new UILayout.
        /// </summary>
        /// <param name="region">The UILayout region.</param>
        /// <param name="layoutType">The lay-outing type.</param>
        /// <returns>The newly created layout.</returns>
        public static UILayout Create(RectangleF region, UILayoutType layoutType)
        {
            // construct new panel
            var layout = new UILayout
            {
                Enabled = true,
                Parent = null,
                Region = region,
                Type = layoutType,
                PreferredSize = new Vector2(region.Width, region.Height),
                ForceExpandHeigth = false,
                ForceExpandWidth = false,
                Padding = new UIPadding(),
                Space = 0,
            };

            return layout;
        }
    }
}
