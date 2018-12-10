// ReCrafted (c) 2016-2018 Always Too Late

using ReCrafted.API.Common;
using ReCrafted.API.Core;
using ReCrafted.API.Mathematics;
using ReCrafted.API.UI;
using ReCrafted.API.UI.Controls;

namespace ReCrafted.Game.Interface
{
    /// <summary>
    /// PauseMenu UI class.
    /// </summary>
    public class PauseMenu : GameSystem
    {
        /// <summary>
        /// Static instance for quick access
        /// </summary>
        public static PauseMenu Instance;

        /// <summary>
        /// Black background of screen.
        /// </summary>
        public UIPanel Background;

        /// <summary>
        /// Region of PauseMenu main panel.
        /// </summary>
        public RectangleF MainPanelRegion => new RectangleF(Display.Width / 2.0f - 75.0f, Display.Height / 2.0f - 100.0f, 150.0f, 200.0f);

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

        // on create
        protected override void OnCreate()
        {
            Instance = this;

            // Create Background (shadow)
            Background = UIPanel.Create(new RectangleF(0, 0, Display.Width, Display.Height), UILayoutType.Vertical, "Pause Background", 9999);
            Background.PanelColor = new Color(0, 0, 0, 0.5f);
            Background.Enabled = false;

            // Create Main Panel
            MainPanel = UIPanel.Create(MainPanelRegion, UILayoutType.Vertical, "Pause Menu", 10000);

            MainPanel.PanelColor = Color.Blue;

            MainPanel.Layout.ForceExpandHeight = true;
            MainPanel.Layout.ForceExpandWidth = true;
            MainPanel.Layout.Padding = new UIPadding
            {
                Top = 10,
                Bottom = 10,
                Left = 10,
                Right = 10
            };
            MainPanel.Layout.Space = 5.0f;
            MainPanel.Layout.Alignment = UILayoutAlignment.MiddleTop;
            MainPanel.ApplyLayout = true;

            // Create Title text
            Title = MainPanel.Add(new UIText());
            Title.PreferredSize = new Vector2(0, 30);
            Title.Text = "ReCrafted";

            MainPanel.AddSpace(10);

            // Create Resume button
            Resume = MainPanel.Add(new UIButton("Resume"));
            Resume.PreferredSize = new Vector2(0, 30);
            Resume.OnClick += GameMain.DisablePause;

            // Create Options button
            Options = MainPanel.Add(new UIButton("Options"));
            Options.PreferredSize = new Vector2(0, 30);

            MainPanel.AddFlexSpace(30);

            // Create Exit button
            Exit = MainPanel.Add(new UIButton("Exit"));
            Exit.PreferredSize = new Vector2(0, 30);
            Exit.OnClick += Application.Exit;

            // Disable Main menu
            Disable();
        }

        // on update
        protected override void OnUpdate()
        {
            if (!Enabled)
                return;
            Background.Region = new RectangleF(0, 0, Display.Width, Display.Height);
            MainPanel.Region = MainPanelRegion;
        }

        /// <summary>
        /// Enable pause menu.
        /// </summary>
        public void Enable()
        {
            MainPanel.Enabled = true;
            Background.Enabled = true;
        }

        /// <summary>
        /// Disable pause menu.
        /// </summary>
        public void Disable()
        {
            MainPanel.Enabled = false;
            Background.Enabled = false;
        }

        /// <summary>
        /// Is pause menu enabled.
        /// </summary>
        public bool Enabled => MainPanel.Enabled;
    }
}
