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

            Layout.Recalculate();
            Layout.Draw();
        }

        // internal
        internal static void DrawAll()
        {
            foreach (var panel in Panels)
            {
                if (!panel.Enabled)
                    continue;

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
                Parent = null,
                Region = region,
                PanelColor = Color.White,
                Layout = new UILayout
                {
                    Type = layoutType,
                    Enabled = true,
                    Region = region,
                    PreferredSize = new Vector2(region.Width, region.Height),
                    ForceExpandHeigth = false,
                    ForceExpandWidth = false
                }
            };
            
            // set panel layout parent
            panel.Layout.Parent = panel;

            // add new panel
            Panels.Add(panel);

            return panel;
        }

        /// <summary>
        /// Contains all controls.
        /// </summary>
        public UILayout Layout { get; private set; }

        /// <summary>
        /// Shows or hides the panel.
        /// </summary>
        public bool Visible { get; set; }

        /// <summary>
        /// Color of the panel.
        /// </summary>
        public Color PanelColor { get; set; }
    }
}
