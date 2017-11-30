// ReCrafted © 2016-2017 Always Too Late

using ReCrafted.API.Common;
using ReCrafted.API.Mathematics;
using ReCrafted.API.UI;
using ReCrafted.API.UI.Controls;

namespace ReCrafted.Game
{
    /// <summary>
    /// PauseMenu UI class
    /// </summary>
    public class PauseMenu : Script
    {
        /// <summary>
        /// Static instance for quick access
        /// </summary>
        public static PauseMenu Instance;

        /// <summary>
        /// PauseMenu's main panel
        /// </summary>
        public UIPanel MainPanel;

        /// <summary>
        /// PauseMenu's title label
        /// </summary>
        public UIText Title;

        /// <summary>
        /// Text that is located below the buttons
        /// </summary>
        public UIText TextBelow;

        /// <summary>
        /// Resume button
        /// </summary>
        public UIButton Resume;

        /// <summary>
        /// Options button
        /// </summary>
        public UIButton Options;

        /// <summary>
        /// Exit button
        /// </summary>
        public UIButton Exit;

        protected internal override void OnCreate()
        {
            Instance = this;

            var rect = new RectangleF(Display.Width / 2.0f - 75.0f, Display.Height / 2.0f - 100.0f, 150.0f, 200.0f);

            // Create Main Panel
            MainPanel = UIPanel.Create(rect, UILayoutType.Vertical, "Pause Menu");

            MainPanel.PanelColor = Color.Blue;

            MainPanel.Layout.ForceExpandHeigth = false;
            MainPanel.Layout.ForceExpandWidth = true;
            MainPanel.Layout.Padding = new UIPadding
            {
                Top = 10,
                Bottom = 10,
                Left = 10,
                Right = 10
            };
            MainPanel.Layout.Space = 5.0f;
            MainPanel.Layout.PreferredSize = new Vector2(30, 30);
            MainPanel.Layout.Alignment = UILayoutAlignment.MiddleTop;
            MainPanel.ApplyLayout = true;

            Title = MainPanel.Add(new UIText());
            Title.Text = "ReCrafted";

            // Create Resume button
            Resume = MainPanel.Add(new UIButton("Resume"));
            Resume.OnClick += GameMain.DisablePause;

            // Create Options button
            Options = MainPanel.Add(new UIButton("Options"));

            // Create Exit button
            Exit = MainPanel.Add(new UIButton("Exit"));
            Exit.OnClick += API.Core.Game.Quit;

            Disable();
        }

        public void Enable()
        {
            MainPanel.Enabled = true;
            MainPanel.Visible = true;
        }

        public void Disable()
        {
            MainPanel.Enabled = false;
            MainPanel.Visible = false;
        }

        public bool Enabled => MainPanel.Enabled;
    }
}
