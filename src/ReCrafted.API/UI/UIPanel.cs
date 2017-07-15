// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

using System.Collections.Generic;
using ReCrafted.API.Core;

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
             Layout.Draw();
        }

        // internal
        internal static void DrawAll()
        {
            foreach (var panel in Panels)
            {
                if (!panel.Visible)
                    continue;

                // temporary
                UIInternal.Color = Color.DarkViolet;
                UIInternal.DrawBox(panel.Region);

                panel.Draw();
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
                Layout = new UILayout
                {
                    Type = layoutType,
                    Enabled = true,
                    Region = region,
                    PreferredSize = new Vector2(100.0f, 100.0f)
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
    }
}
