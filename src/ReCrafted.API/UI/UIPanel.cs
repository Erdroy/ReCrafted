// ReCrafted © 2016-2017 Always Too Late

using System;
using System.Collections.Generic;
using ReCrafted.API.Common;
using ReCrafted.API.Core;
using ReCrafted.API.Mathematics;

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
        internal static bool HaveColision;

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
                if (ApplyLayout)
                {
                    Profiler.BeginProfile("Layout.Recalculate");
                    Layout.Recalculate(Region);
                    Profiler.EndProfile();
                }
                Profiler.BeginProfile("Layout.Draw");
                //draw layout
                Layout.Draw();
                Profiler.EndProfile();
                //calculate mouse collisions
                if (!HaveColision)
                {
                    Profiler.BeginProfile("Layout.LookForMouseCollision");
                    var collision = Layout.LookForMouseCollision();
                    if (Input.IsKeyDown(Keys.Mouse0))
                    {
                        if (collision != null)
                        {
                            HaveColision = true;
                            collision.OnMouseClick();
                            if (FocusedControl != collision)
                                SetFocusedControl(collision);
                        }
                    }
                    Profiler.EndProfile();
                }
                Profiler.EndProfile();
            }
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

        /// <summary>
        /// Draws all panels.
        /// </summary>
        internal static void DrawAll()
        {
            // reset
            HaveColision = false;
            foreach (var panel in Panels)
            {
                try
                {
                    panel.Draw();
                }
                catch (Exception ex)
                {
                    Logger.Write(ex.ToString(), LogLevel.Error);
                }
            }
            if (Input.IsKeyDown(Keys.Mouse0) && !HaveColision)
                SetFocusedControl(null);
        }

        /// <summary>
        /// Creates new UIPanel.
        /// </summary>
        /// <param name="region">The UIPanel region.</param>
        /// <param name="layoutType">The lay-outing type.</param>
        /// <param name="name">Name of UIPanel.</param>
        /// <returns>The newly created panel.</returns>
        public static UIPanel Create(RectangleF region, UILayoutType layoutType, string name = "")
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
                Depth = 1 + (Panels.Count == 0 ? 0 : Panels[Panels.Count - 1].Depth) +
                        (Panels.Count == 0 ? 0 : Panels[Panels.Count - 1].Layout.Controls.Count),
                Layout = UILayout.Create(region, layoutType),
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
        /// Layout of this panel will be applied in to children's.
        /// </summary>
        public bool ApplyLayout { get; set; }

        /// <summary>
        /// Panel is visible.
        /// </summary>
        public bool Visible { get; set; }

        /// <summary>
        /// Color of the panel.
        /// </summary>
        public Color PanelColor { get; set; }

        /// <summary>
        /// Contains all controls.
        /// </summary>
        public UILayout Layout { get; protected set; }
    }
}
