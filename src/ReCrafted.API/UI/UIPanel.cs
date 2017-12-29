// ReCrafted © 2016-2017 Always Too Late

using System;
using System.Collections.Generic;
using ReCrafted.API.Common;
using ReCrafted.API.Core;
using ReCrafted.API.Mathematics;
using ReCrafted.API.UI.Controls;

namespace ReCrafted.API.UI
{
    /// <summary>
    /// UIPanel class, implements base panel class for all controls.
    /// Allows to dynamically show and hide UILayout elements.
    /// </summary>
    public class UIPanel : UIControl
    {
        // actual list of all panels in ui.
        private static readonly List<UIPanel> Panels = new List<UIPanel>();

        // is mouse collide with any control of any panel?
        internal static bool HaveCollision;

        private bool _fixed;

        // are scroll bars enabled?
        private bool _enableScrollbars;

        // internal panel for scroll bars
        private UIPanel _internalPanel;

        // vertical scroll bar
        private UIScrollbar _verticalScrollbar;

        // vertical button top
        private UIButton _verticalButtonTop;

        // vertical button bottom
        private UIButton _verticalButtonBottom;

        // horizontal scroll bar
        private UIScrollbar _horizotnalScrollbar;

        // position of current scroll view
        private Vector2 _scrollViewPosition;

        // position of current scroll view
        private Vector2 _fixedScrollViewPosition;

        // display of scroll view
        private RectangleF _scrollViewDisplay;

        public override void OnMouseOver()
        {
            if (EnableScrollBars)
            {
                if (VerticalScrollBar)
                {
                    _verticalScrollbar.Position += 0; // scroll input here
                    _scrollViewPosition.Y = _scrollViewDisplay.Height * _verticalScrollbar.Position;
                }
                else if (HorizontalScrollBar)
                {
                    _horizotnalScrollbar.Position += 0; // scroll input here
                    _scrollViewPosition.X = _scrollViewDisplay.Width * _horizotnalScrollbar.Position;
                }
            }
        }

        /// <summary>
        /// Draws all controls added to this UIPanel.
        /// </summary>
        public override void Draw()
        {
            if (Enabled)
            {
                UIInternal.Depth = Depth;
                UIInternal.Color = PanelColor;
                UIInternal.DrawBox(Region);

                Profiler.BeginProfile($"Panel <{(string.IsNullOrEmpty(Name) ? "empty" : Name)}> ");

                //recalculate current layout
                if (EnableScrollBars)
                {
                    _scrollViewPosition = new Vector2(HorizontalScrollBar ? _scrollViewPosition.X : 0,
                        VerticalScrollBar ? _scrollViewPosition.Y : 0);
                    _fixedScrollViewPosition = Vector2.Lerp(_fixedScrollViewPosition, _scrollViewPosition,
                        (float) Time.DeltaTime * 10f);
                }

                RectangleF layoutRegion;
                if (EnableScrollBars)
                {
                    layoutRegion = new RectangleF(Region.X + _fixedScrollViewPosition.X,
                        Region.Y + _fixedScrollViewPosition.Y,
                        Region.Width - (VerticalScrollBar ? ScrollBarsSize : 0),
                        Region.Height - (HorizontalScrollBar ? ScrollBarsSize : 0));
                }
                else
                {
                    layoutRegion = Region;
                }

                var scrollRegion = new RectangleF(Region.X, Region.X,
                    Region.Width - (VerticalScrollBar ? ScrollBarsSize : 0),
                    Region.Height - (HorizontalScrollBar ? ScrollBarsSize : 0));

                var recalculatedLayout = new RectangleF();
                if (ApplyLayout)
                {
                    recalculatedLayout = Layout.Recalculate(layoutRegion);
                }

                layoutRegion = recalculatedLayout == new RectangleF() ? layoutRegion : recalculatedLayout;
                if (EnableClipping)
                {
                    UIInternal.BeginViewRect(EnableScrollBars ? scrollRegion : layoutRegion);
                }

                //draw layout
                Layout.Draw();

                if (EnableScrollBars)
                {
                    var displayHeight = 0f;
                    var displayWidth = 0f;

                    foreach (var c in Layout.Controls)
                    {
                        displayHeight += c.Region.Height + Math.Abs(Layout.Space) / 2f;
                        if (c.Region.Width > displayWidth)
                            displayWidth = c.Region.Width;
                    }

                    _scrollViewDisplay = new RectangleF(layoutRegion.X,
                        layoutRegion.Y + layoutRegion.Height - displayHeight, displayWidth, displayHeight);

                    // debug
                    // UIInternal.Color = Color.Green;
                    // UIInternal.Depth = 99999;
                    // UIInternal.DrawBox(_scrollViewDisplay);

                    var verticalSize = MathUtil.Clamp(scrollRegion.Height / _scrollViewDisplay.Height, 0.05f, 1f);
                    _verticalScrollbar.Size = verticalSize;

                    //var p = new Vector2(_scrollViewDisplay.X, _scrollViewDisplay.Y);
                    //UIInternal.Color = Color.White;
                    //UIInternal.DrawString(DefaultFont.NativePtr, _scrollViewDisplay.ToString(), ref p);
                }

                //calculate mouse collisions
                if (!HaveCollision)
                {

                    var collision = Layout.LookForMouseCollision();
                    if (Input.IsKeyDown(Keys.Mouse0))
                    {
                        if (collision != null)
                        {
                            HaveCollision = true;
                            collision.OnMouseClick();
                            if (FocusedControl != collision)
                                SetFocusedControl(collision);
                        }
                        else
                        {
                            _checkForPanelCollision();
                        }
                    }
                }

                if (!_fixed)
                {
                    Reset();
                    _fixed = true;
                }

                if (EnableClipping)
                    UIInternal.EndViewRect();

                Profiler.EndProfile();

                if (EnableScrollBars)
                {
                    Profiler.BeginProfile($"Panel <{(string.IsNullOrEmpty(Name) ? "empty" : Name)}> ");

                    _horizotnalScrollbar.Enabled = HorizontalScrollBar;
                    if (HorizontalScrollBar)
                        _horizotnalScrollbar.Region = new RectangleF(scrollRegion.X,
                            scrollRegion.Y + scrollRegion.Height, scrollRegion.Width, ScrollBarsSize);

                    _verticalScrollbar.Enabled = VerticalScrollBar;
                    if (VerticalScrollBar)
                    {
                        _verticalScrollbar.Region = new RectangleF(scrollRegion.X + scrollRegion.Width,
                            scrollRegion.Y + ScrollBarsSize,
                            ScrollBarsSize, scrollRegion.Height - ScrollBarsSize * 2);
                        _verticalButtonTop.Region = new RectangleF(scrollRegion.X + scrollRegion.Width,
                            scrollRegion.Y, ScrollBarsSize, ScrollBarsSize);
                        if (!HorizontalScrollBar)
                        {
                            _verticalButtonBottom.Region = new RectangleF(scrollRegion.X + scrollRegion.Width,
                                scrollRegion.Y + scrollRegion.Height - ScrollBarsSize, ScrollBarsSize,
                                ScrollBarsSize);
                        }
                    }

                    _internalPanel.Draw();
                    Profiler.EndProfile();
                }
            }
        }

        // reset
        public override void Reset()
        {
            Layout?.Reset();
            _internalPanel?.Reset();
        }

        /// <summary>
        /// Add new control in to layout container of this panel.
        /// </summary>
        /// <typeparam name="T">Control class which must inherit from UIControl.</typeparam>
        /// <param name="instance">The control instance.</param>
        /// <returns>The control instance, allows to modify preferences easily.</returns>
        /// <exception cref="System.ArgumentNullException">Exception is thrown when given instance of control is null.</exception>
        /// <exception cref="ReCraftedException">Exception is thrown when given instance of control already exists in container.</exception>
        public T Add<T>(T instance) where T : UIControl
        {
            return Layout.Add(instance);
        }

        /// <summary>
        /// Removes given control from the layout container of this panel.
        /// </summary>
        /// <typeparam name="T">Control class which must inherit from UIControl.</typeparam>
        /// <param name="instance">The control instance.</param>
        /// <exception cref="ArgumentNullException">Exception is thrown when givent instance of controll is null.</exception>
        public void Remove<T>(T instance) where T : UIControl
        {
            Layout.Remove(instance);
        }

        // checks if mouse collide with this panel
        private void _checkForPanelCollision()
        {
            if (!Region.Contains(Input.CursorPosition)) return;
            HaveCollision = true;
            OnMouseClick();
            if (FocusedControl != this)
                SetFocusedControl(this);
        }

        // hide scroll bars if exists
        private void _hideScrollbars()
        {
            if (_internalPanel != null)
            {
                _internalPanel.Enabled = false;
            }
        }

        // create scroll bars if don't exists
        private void _makeScrollBars()
        {
            if (_internalPanel != null)
            {
                _internalPanel.Enabled = true;
            }
            else
            {
                _internalPanel = Create(Region, UILayoutType.Vertical, Name + "<Internal>", Depth);
                _internalPanel.ApplyLayout = false;
                _internalPanel.PanelColor = Color.Transparent;
                _internalPanel.Parent = this;
                _horizotnalScrollbar = _internalPanel.Add(new UIScrollbar());
                _horizotnalScrollbar.Vertical = false;
                _horizotnalScrollbar.OnValueChanged += position =>
                {
                    if (_horizotnalScrollbar.IsDragging)
                    {
                        _scrollViewPosition.X = _scrollViewDisplay.Width * position;
                    }
                };

                _verticalScrollbar = _internalPanel.Add(new UIScrollbar());
                _verticalScrollbar.Vertical = true;
                _verticalScrollbar.OnHandleChanged += position =>
                {
                    if (_verticalScrollbar.IsDragging)
                    {
                        _scrollViewPosition.Y = _scrollViewDisplay.Height * position;
                    }
                };

                _verticalButtonTop = _internalPanel.Add(new UIButton(new RectangleF(), @"/\", Color.DarkOrange,
                    UIControlColors.DefaultHandle));
                _verticalButtonTop.OnClick += () =>
                {
                    _verticalScrollbar.Position = 0f;
                    _scrollViewPosition.Y = 0f;
                };
                _verticalButtonBottom = _internalPanel.Add(new UIButton(new RectangleF(), @"\/", Color.DarkOrange,
                    UIControlColors.DefaultHandle));
                /*
                _verticalButtonBottom.OnClick += () =>
                {
                    _verticalScrollbar.Position = 1f;
                    _scrollViewPosition.Y = _scrollViewDisplay.Height;
                };
                */
            }
        }

        /// <summary>
        /// Draws all panels.
        /// </summary>
        internal static void DrawAll()
        {
            // reset
            HaveCollision = false;
            for (var index = Panels.Count - 1; index >= 0; index--)
            {
                try
                {
                    if (Panels[index].Parent != null)
                        continue;

                    Panels[index].Draw();
                }
                catch (Exception ex)
                {
                    Logger.Write(ex.ToString(), LogLevel.Error);
                }
            }
            if (Input.IsKeyDown(Keys.Mouse0) && !HaveCollision)
                SetFocusedControl(null);
        }

        /// <summary>
        /// Creates new UIPanel.
        /// </summary>
        /// <param name="region">The UIPanel region.</param>
        /// <param name="layoutType">The lay-outing type.</param>
        /// <param name="name">Name of UIPanel.</param>
        /// <param name="baseDepth">Base depth of panel.</param>
        /// <returns>The newly created panel.</returns>
        public static UIPanel Create(RectangleF region, UILayoutType layoutType, string name = "", int baseDepth = 1)
        {
            // construct new panel
            var panel = new UIPanel
            {
                Name = name,
                Enabled = true,
                ApplyLayout = true,
                Parent = null,
                Region = region,
                PanelColor = Color.White,
                Depth = baseDepth + (Panels.Count == 0 ? 0 : Panels[Panels.Count - 1].Depth) +
                        (Panels.Count == 0 ? 0 : Panels[Panels.Count - 1].Layout.Controls.Count),
                Layout = UILayout.Create(region, layoutType),
                EnableClipping = false,
                EnableScrollBars = false,
                HorizontalScrollBar = true,
                VerticalScrollBar = true
            };

            panel._enableScrollbars = false;
            panel._scrollViewPosition = Vector2.Zero;

            panel._fixedScrollViewPosition = Vector2.Zero;
            panel._scrollViewDisplay = new RectangleF();

            // set panel layout parent
            panel.Layout.Parent = panel;
            // apply panel depth
            panel.Layout.Depth = panel.Depth + 1;

            // recalculate whole depth when controls in panel has been changed.
            panel.Layout.OnControlsChanged += () =>
            {
                for (var index = Panels.IndexOf(panel); index < Panels.Count; index++)
                {
                    Panels[index].Depth =
                        1 + (index == 0 ? 0 : Panels[index - 1].Depth + Panels[index - 1].Layout.Controls.Count);
                    Panels[index].Layout.Depth = Panels[index].Depth;
                    Panels[index].Layout.RecalculateDepth();
                }
            };

            // add new panel
            Panels.Add(panel);

            return panel;
        }

        /// <summary>
        /// Layout of this panel will be applied in to children.
        /// </summary>
        public bool ApplyLayout { get; set; }

        /// <summary>
        /// Color of the panel.
        /// </summary>
        public Color PanelColor { get; set; }

        /// <summary>
        /// Enable content clipping.
        /// </summary>
        public bool EnableClipping { get; set; }

        /// <summary>
        /// Enable scroll bars.
        /// </summary>
        public bool EnableScrollBars
        {
            get { return _enableScrollbars; }
            set
            {
                var p = _enableScrollbars;
                _enableScrollbars = value;
                if (p == _enableScrollbars) return;
                if (_enableScrollbars)
                {
                    _makeScrollBars();
                }
                else
                {
                    _hideScrollbars();
                }
            }
        }

        /// <summary>
        /// Horizontal scroll bar enable state.
        /// </summary>
        public bool HorizontalScrollBar { get; set; }

        /// <summary>
        /// Vertical scroll bar enable state.
        /// </summary>
        public bool VerticalScrollBar { get; set; }

        /// <summary>
        /// Size of scroll bars.
        /// </summary>
        public int ScrollBarsSize = 20;

        /// <summary>
        /// Contains all controls.
        /// </summary>
        public UILayout Layout { get; protected set; }

        /// <summary>
        /// Vertical scroll bar.
        /// </summary>
        public UIScrollbar VerticalScrollbar => _verticalScrollbar;

        /// <summary>
        /// Top vertical button.
        /// </summary>
        public UIButton VerticalTopButton => _verticalButtonTop;

        /// <summary>
        /// Bottom vertical button.
        /// </summary>
        public UIButton VerticalBottomButton => _verticalButtonBottom;

        /// <summary>
        /// Horizontal scroll bar.
        /// </summary>
        public UIScrollbar HorizontalScrollbar => _horizotnalScrollbar;
    }
}
