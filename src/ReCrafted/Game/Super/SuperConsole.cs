// ReCrafted (c) 2016-2018 Always Too Late

using System;
using ReCrafted.API.Common;
using ReCrafted.API.Core;
using ReCrafted.API.Mathematics;
using ReCrafted.API.UI;
using ReCrafted.API.UI.Controls;

namespace ReCrafted.Game.Super
{
    /// <summary>
    /// In-Game console UI class.
    /// </summary>
    public class SuperConsole : Script
    {
        /// <summary>
        /// Static instance for quick access
        /// </summary>
        public static SuperConsole Instance;

        /// <summary>
        /// Region of SuperConsole text container.
        /// </summary>
        public RectangleF TextContainerRegion => new RectangleF(0, 0, Display.Width, Display.Height / 4f);

        /// <summary>
        /// SuperConsole text container.
        /// </summary>
        public UIPanel TextContainer;

        /// <summary>
        /// Region of SuperConsole input.
        /// </summary>
        public RectangleF InputRegion => new RectangleF(0, Display.Height / 4f, Display.Width, 25);

        /// <summary>
        /// Input text field of super console.
        /// </summary>
        public UITextField Input;

        // on create
        protected internal override void OnCreate()
        {
            Instance = this;

            // Create Main Panel
            TextContainer = UIPanel.Create(TextContainerRegion, UILayoutType.Vertical, "Super Console", 10001);

            TextContainer.PanelColor = Color.DarkGray * 2f;
            TextContainer.EnableClipping = true;
            TextContainer.EnableScrollBars = true;
            TextContainer.EnableHorizontalScrollbar = false;
            TextContainer.EnableVerticalScrollbar = true;

            TextContainer.EnableScrollButtons = false;

            TextContainer.Layout.ReverseContainer = true;
            TextContainer.Layout.PreferredSize = new Vector2(0, 30);
            TextContainer.Layout.ForceExpandHeight = false;
            TextContainer.Layout.ForceExpandWidth = true;
            TextContainer.Layout.Padding = new UIPadding
            {
                Top = 5,
                Bottom = 5,
                Left = 5,
                Right = 5
            };
            TextContainer.Layout.Space = -UIControl.DefaultFont.Size / 1.5f;
            TextContainer.Layout.Alignment = UILayoutAlignment.LeftBottom;
            TextContainer.Layout.Offset = new Vector2(0, -10);
            TextContainer.ApplyLayout = true;

            Input = UIControl.CreateControl(new UITextField(InputRegion, string.Empty));

            Input.SpringAnimation = false;
            Input.MultipleLine = false;
            Input.OnSubmit += OnInputSubmit;

            // init super commands
            SuperCommands.Init();

            // register default commands
            SuperDefaults.RegisterSuperDefaults();

            // handle logger
            Logger.OnMessage += OnLoggerMessage;

            // hide console
            Disable();
        }

        protected override void OnDestroy()
        {
            Logger.OnMessage -= OnLoggerMessage;
        }

        // logger message callback
        private void OnLoggerMessage(string message, LogLevel level)
        {
            if (message.EndsWith('\n'.ToString()))
                message = message.Remove(message.Length - 2, 2);

            Write(message, level);
        }

        // on input submit
        private void OnInputSubmit()
        {
            if (string.IsNullOrEmpty(Input.Text) || string.IsNullOrWhiteSpace(Input.Text))
            {
                Input.Text = string.Empty;
                return;
            }

            var t = Write($"> {Input.Text}");
            t.TextColor = Color.LimeGreen;
            SuperCommands.Instance.ExecuteRaw(Input.Text);
            Input.Text = string.Empty;
        }

        // on update
        protected override void OnUpdate()
        {
            if (!Enabled)
                return;

            TextContainer.Region = TextContainerRegion;
            Input.Region = InputRegion;
        }

        /// <summary>
        /// Enables super console.
        /// </summary>
        public void Enable()
        {
            TextContainer.Enabled = true;
            Input.Enabled = true;

            UIControl.SetFocusedControl(Input);
        }

        /// <summary>
        /// Disable super console.
        /// </summary>
        public void Disable()
        {
            TextContainer.Enabled = false;
            Input.Enabled = false;
        }

        /// <summary>
        /// Writes text to console.
        /// </summary>
        /// <param name="text">Text to write.</param>
        /// <param name="level">Level of log.</param>
        public static UIText Write(string text, LogLevel level = LogLevel.Info)
        {
            var uiText = Instance.TextContainer.Add(new UIText(string.Empty));
            uiText.FitRegionSizeToText = true;
            switch (level)
            {
                case LogLevel.Info:
                    uiText.TextColor = Color.White;
                    break;
                case LogLevel.Warning:
                    uiText.TextColor = Color.Yellow;
                    break;
                case LogLevel.Error:
                    uiText.TextColor = Color.Red;
                    break;
                case LogLevel.Fatal:
                    uiText.TextColor = Color.DarkRed;
                    break;
                default:
                    throw new ArgumentOutOfRangeException(nameof(level), level, null);
            }

            uiText.Text = text;

            return uiText;
        }

        /// <summary>
        /// Clears console.
        /// </summary>
        public static void Clear()
        {
            while (Instance.TextContainer.Layout.Controls.Count > 0)
            {
                Instance.TextContainer.Layout.Remove(Instance.TextContainer.Layout.Controls[0]);
            }
        }

        /// <summary>
        /// Is super console enabled?
        /// </summary>
        public bool Enabled => TextContainer.Enabled;
    }
}
