// ReCrafted © 2016-2017 Always Too Late

using System;
using System.Collections.Generic;
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

        /// <summary>
        /// Draws all controls added to this UIPanel.
        /// </summary>
        public override void Draw()
        {
            if (Visible)
            {
                UIInternal.Color = PanelColor;
                UIInternal.DrawBox(Region);
            }

            if (Enabled)
            {
                //recalculate current layout
                if (ApplyLayout)
                    Layout.Recalculate(Region);
                //draw layout
                Layout.Draw();
                //calculate mouse collisions
                Layout.LookForMouseCollision();
            }
        }

        /// <summary>
        /// Draws all panels.
        /// </summary>
        internal static void DrawAll()
        {
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
        }

        /// <summary>
        /// Creates new UIPanel.
        /// </summary>
        /// <param name="region">The UIPanel region.</param>
        /// <param name="layoutType">The lay-outing type.</param>
        /// <returns>The newly created panel.</returns>
        public static UIPanel Create(RectangleF region, UILayoutType layoutType)
        {
            // construct new panel
            var panel = new UIPanel
            {
                Enabled = true,
                Visible = true,
                ApplyLayout = true,
                Parent = null,
                Region = region,
                PanelColor = Color.White,
                Layout = UILayout.Create(region, layoutType)
            };
            
            // set panel layout parent
            panel.Layout.Parent = panel;

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
