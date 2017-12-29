// ReCrafted © 2016-2017 Always Too Late

using System;
using System.Collections.Generic;
using System.Linq;
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

        // last collision of mouse on control of any panel
        internal static UIControl LastCollision;

        private bool _fixed;

        // are scroll bars enabled?
        private bool _enableScrollbars;

        // internal panel for scroll bars
        internal UIPanel _internalPanel;

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

        // region of layout
        private RectangleF _scrollLayoutRegion;

        // region of scroll view
        private RectangleF _scrollRegion;

        // size of scroll bar;
        private int _scrollBarsSize;

        // region of panel updated every frame, to check for changes
        private RectangleF _previousRegion;

        // on mouse over
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
                Profiler.BeginProfile($"Panel <{(string.IsNullOrEmpty(Name) ? "empty" : Name)}> ");

                UIInternal.Depth = Depth;
                UIInternal.Color = PanelColor;
                UIInternal.DrawBox(Region);

                if (_previousRegion != Region)
                {
                    _previousRegion = Region;
                    OnChanged();
                }

                // update scroll view position
                if (EnableScrollBars)
                {
                    _scrollViewPosition = new Vector2(HorizontalScrollBar ? _scrollViewPosition.X : 0,
                        VerticalScrollBar ? _scrollViewPosition.Y : 0);
                    _fixedScrollViewPosition = Vector2.Lerp(_fixedScrollViewPosition, _scrollViewPosition,
                        (float) Time.DeltaTime * 10f);
                }

                // recalculate current layout
                var recalculatedLayout = new RectangleF();
                if (ApplyLayout)
                {
                    recalculatedLayout = Layout.Recalculate(EnableScrollBars ? _scrollLayoutRegion : Region);
                }

                // handle clipping
                if (EnableClipping)
                {
                    //UIInternal.Depth = UIInternal.Depth + 13;
                    var region = EnableScrollBars ? _scrollRegion : (recalculatedLayout == new RectangleF() ? Region : recalculatedLayout);
                    //UIInternal.DrawBox(region);
                    UIInternal.BeginViewRect(region);
                }

                //draw layout
                Layout.Draw();

                //calculate mouse collisions
                if (!HaveCollision)
                {
                    var currentCollision = Layout.LookForMouseCollision();
                    if (Input.IsKeyDown(Keys.Mouse0))
                    {
                        if (currentCollision != null)
                        {
                            HaveCollision = true;
                            currentCollision.OnMouseClick();
                            if (FocusedControl != currentCollision)
                                SetFocusedControl(currentCollision);
                        }
                        else
                        {
                            _checkForPanelCollision();
                        }
                    }

                    if (currentCollision != null)
                        LastCollision = currentCollision;
                }

                // fix
                if (!_fixed)
                {
                    Reset();
                    _fixed = true;
                }

                // handle clipping
                if (EnableClipping)
                    UIInternal.EndViewRect();

                Profiler.EndProfile();

                // handle scroll bars
                if (EnableScrollBars)
                {
                    Profiler.BeginProfile($"Panel <{(string.IsNullOrEmpty(_internalPanel.Name) ? "empty" : _internalPanel.Name)}> ");

                    _horizotnalScrollbar.Enabled = HorizontalScrollBar;
                    if (HorizontalScrollBar)
                        _horizotnalScrollbar.Region = new RectangleF(_scrollRegion.X,
                            _scrollRegion.Y + _scrollRegion.Height, _scrollRegion.Width, _scrollBarsSize);

                    _verticalScrollbar.Enabled = VerticalScrollBar;
                    if (VerticalScrollBar)
                        _verticalScrollbar.Region = new RectangleF(_scrollRegion.X + _scrollRegion.Width,
                            _scrollRegion.Y + _scrollBarsSize,
                            _scrollBarsSize, _scrollRegion.Height - _scrollBarsSize * 2);
      
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
        /// Adds space between last an next control of layout container of this panel.
        /// </summary>
        /// <param name="space">Space amount.</param>
        public void AddSpace(int space)
        {
            Layout.Add(new UIBox
            {
                BoxColor = Color.Transparent,
                IgnoreMouseCollision = true,
                PreferredSize = new Vector2(space, space)
            });
        }

        /// <summary>
        /// Adds flexable space between last an next control of layout container of this panel.
        /// Flexable space works only if layout container have enabled force expand option.
        /// </summary>
        /// <param name="nextControlSize">Size of next control after flex space. By default (zero value) flex size is the average control size of layout force expand.</param>
        public void AddFlexSpace(int nextControlSize = 0)
        {
            var preferredSize = new Vector2(0, 0);
            if (nextControlSize != 0)
            {
                switch (Layout.Type)
                {
                    case UILayoutType.Grid:
                        throw new NotImplementedException("Grid layout type is not yet implemented.");
                    case UILayoutType.DynamicGrid:
                        throw new NotImplementedException("DynamicGrid layout type is not yet implemented.");
                    case UILayoutType.Horizontal:
                        var totalWidth = Layout.Controls.Sum(c => c.Region.Width + Layout.Space * 1.5f);
                        var width = Layout.Region.Width - totalWidth - nextControlSize;
                        if (width > 0.01f)
                            preferredSize = new Vector2(width, 0);
                        break;
                    case UILayoutType.Vertical:
                        var totalHeight = Layout.Controls.Sum(c => c.Region.Height + Layout.Space * 1.5f);
                        var height = Layout.Region.Height - totalHeight - nextControlSize;
                        if (height > 0.01f)
                            preferredSize = new Vector2(0, height);
                        break;
                    default:
                        throw new ArgumentOutOfRangeException();
                }
            }

            Layout.Add(new UIBox
            {
                BoxColor = Color.Transparent,
                IgnoreMouseCollision = true,
                PreferredSize = preferredSize
            });
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

        // content of panel's layout or panel's region has been changed 
        private void OnChanged()
        {
            if (!EnableScrollBars)
                return;

            // recalculate layout region
            _scrollLayoutRegion = new RectangleF(Region.X + _fixedScrollViewPosition.X,
                Region.Y + _fixedScrollViewPosition.Y,
                Region.Width - (VerticalScrollBar ? _scrollBarsSize : 0),
                Region.Height - (HorizontalScrollBar ? _scrollBarsSize : 0));

            // recalculate scroll region
            _scrollRegion = new RectangleF(Region.X, Region.Y,
                Region.Width - (VerticalScrollBar ? _scrollBarsSize : 0),
                Region.Height - (HorizontalScrollBar ? _scrollBarsSize : 0));

            // recalculate size of scroll view display and apply new size of scroll bars handle

            var displayHeight = 0f;
            var displayWidth = 0f;

            foreach (var c in Layout.Controls)
            {
                displayHeight += c.Region.Height + Math.Abs(Layout.Space) / 2f;
                if (c.Region.Width > displayWidth)
                    displayWidth = c.Region.Width;
            }

            _scrollViewDisplay = new RectangleF(_scrollLayoutRegion.X, _scrollLayoutRegion.Y + _scrollLayoutRegion.Height - displayHeight,
                displayWidth, displayHeight);

            var verticalSize = MathUtil.Clamp(_scrollRegion.Height / _scrollViewDisplay.Height, 0.05f, 1f);
            _verticalScrollbar.Size = verticalSize;

            // update buttons
            if (VerticalScrollBar)
            {
                _verticalButtonTop.Region = new RectangleF(_scrollRegion.X + _scrollRegion.Width,
                    _scrollRegion.Y, _scrollBarsSize, _scrollBarsSize);
                if (!HorizontalScrollBar)
                {
                    _verticalButtonBottom.Region = new RectangleF(_scrollRegion.X + _scrollRegion.Width,
                        _scrollRegion.Y + _scrollRegion.Height - _scrollBarsSize, _scrollBarsSize,
                        _scrollBarsSize);
                }
            }
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

                    OnChanged();
                };

                _verticalButtonTop = _internalPanel.Add(new UIButton(new RectangleF(), string.Empty, Color.DarkOrange,
                    UIControlColors.DefaultHandle, DefaultArrowUp));
                _verticalButtonTop.OnClick += () =>
                {
                    _verticalScrollbar.Position = 0f;
                    _scrollViewPosition.Y = 0f;
                };
                _verticalButtonBottom = _internalPanel.Add(new UIButton(new RectangleF(), string.Empty, Color.DarkOrange,
                    UIControlColors.DefaultHandle, DefaultArrowDown));
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
                ScrollBarSize = 20,
                HorizontalScrollBar = true,
                VerticalScrollBar = true,

                _enableScrollbars = false,
                _scrollViewPosition = Vector2.Zero,
                _fixedScrollViewPosition = Vector2.Zero,
                _scrollViewDisplay = new RectangleF()
            };

            panel._scrollBarsSize = panel.ScrollBarSize;

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

            panel.Layout.OnControlsChanged += panel.OnChanged;

            panel.OnChanged();

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

                OnChanged();
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
        public int ScrollBarSize {
            get { return _scrollBarsSize; }
            set
            {
                _scrollBarsSize = value;
                OnChanged();
            }
        }

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
