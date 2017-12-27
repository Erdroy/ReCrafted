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

        // is panel visible?
        private bool _visible;
        private bool _fixed;

        // are scroll bars enabled?
        private bool _enableScrollbars;

        // internal panel for scroll bars
        private UIPanel _internalPanel;

        // vertical scroll bar
        private UIScrollbar _verticalScrollbar;

        // horizontal scroll bar
        private UIScrollbar _horizotnalScrollbar;

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
            }

            if (Visible)
            {
                Profiler.BeginProfile($"Panel <{(string.IsNullOrEmpty(Name) ? "empty" : Name)}> ");

                //recalculate current layout
                var region = EnableScrollBars ? new RectangleF(Region.X, Region.X, 
                    Region.Width - (VerticalScrollBar ? ScrollBarsSize : 0), 
                    Region.Height - (HorizontalScrollBar ?  ScrollBarsSize : 0)) : Region;
                if (ApplyLayout)
                {
                    var r = Layout.Recalculate(region);
                    if (EnableClipping)
                    {
                        UIInternal.BeginViewRect(r);
                    }
                }
                else
                {
                    if (EnableClipping)
                    {
                        UIInternal.BeginViewRect(region);
                    }
                }

                //draw layout
                Layout.Draw();
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
                {
                    UIInternal.EndViewRect();
                }
                Profiler.EndProfile();

                if (Enabled && EnableScrollBars)
                {
                    Profiler.BeginProfile($"Panel <{(string.IsNullOrEmpty(Name) ? "empty" : Name)}> ");
                    _horizotnalScrollbar.Enabled = HorizontalScrollBar;
                    if (HorizontalScrollBar)
                        _horizotnalScrollbar.Region = new RectangleF(region.X, region.Y + region.Height, region.Width, ScrollBarsSize);

                    _verticalScrollbar.Enabled = VerticalScrollBar;
                    if (VerticalScrollBar)
                        _verticalScrollbar.Region = new RectangleF(region.X + region.Width, region.Y, ScrollBarsSize, region.Height);

                    _internalPanel.Draw();
                    Profiler.EndProfile();
                }
            }
            else
            {
                if (Enabled)
                    _checkForPanelCollision();
            }
        }

        // reset
        public override void Reset()
        {
            Layout?.Reset();
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
                _internalPanel.Visible = false;
            }
        }

        // create scroll bars if don't exists
        private void _makeScrollBars()
        {
            if (_internalPanel != null)
            {
                _internalPanel.Enabled = true;
                _internalPanel.Visible = true;
            }
            else
            {
                _internalPanel = Create(Region, UILayoutType.Vertical, Name + "<Internal>", Depth);
                _internalPanel.ApplyLayout = false;
                _internalPanel.PanelColor = Color.Transparent;
                _internalPanel.Parent = this;
                _horizotnalScrollbar = _internalPanel.Add(new UIScrollbar());
                _horizotnalScrollbar.Vertical = false;

                _verticalScrollbar = _internalPanel.Add(new UIScrollbar());
                _verticalScrollbar.Vertical = true;
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
                Visible = true,
                ApplyLayout = true,
                Parent = null,
                Region = region,
                PanelColor = Color.White,
                Depth = baseDepth + (Panels.Count == 0 ? 0 : Panels[Panels.Count - 1].Depth) + (Panels.Count == 0 ? 0 : Panels[Panels.Count - 1].Layout.Controls.Count),
                Layout = UILayout.Create(region, layoutType),
                EnableClipping = false,
                EnableScrollBars = false,
                HorizontalScrollBar = true,
                VerticalScrollBar = true
            };

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
        /// Creates new panel with control instance.
        /// </summary>
        /// <typeparam name="T">Type of control.</typeparam>
        /// <param name="region">Region of panel with control.</param>
        /// <param name="controlInstance">Control instance.</param>
        public static void CreateControl<T>(RectangleF region, ref T controlInstance) where T : UIControl
        {
            var panel = Create(region, UILayoutType.Vertical, $"Control-{controlInstance.Name}");
            panel.Layout.ForceExpandHeight = true;
            panel.Layout.ForceExpandWidth = true;
            panel.ApplyLayout = true;

            controlInstance = panel.Add(controlInstance);
        }

        /// <summary>
        /// Creates new panel with control instance.
        /// </summary>
        /// <typeparam name="T">Type of control.</typeparam>
        /// <param name="region">Region of panel with control.</param>
        /// <param name="controlInstance">Control instance.</param>
        /// <param name="panel">Created instance of UIPanel.</param>
        public static void CreateControl<T>(RectangleF region, ref T controlInstance, out UIPanel panel) where T : UIControl
        {
            panel = Create(region, UILayoutType.Vertical, $"Control-{controlInstance.Name}");
            panel.Layout.ForceExpandHeight = true;
            panel.Layout.ForceExpandWidth = true;
            panel.ApplyLayout = true;

            controlInstance = panel.Add(controlInstance);
        }

        /// <summary>
        /// Layout of this panel will be applied in to children.
        /// </summary>
        public bool ApplyLayout { get; set; }

        /// <summary>
        /// Panel is visible.
        /// </summary>
        public bool Visible
        {
            get { return _visible; }
            set
            {
                _visible = value;
                if (_visible)
                    _fixed = false;
            }
        }

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
    }
}
