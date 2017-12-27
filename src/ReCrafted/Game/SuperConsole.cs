// ReCrafted © 2016-2017 Always Too Late

using System;
using ReCrafted.API.Common;
using ReCrafted.API.Mathematics;
using ReCrafted.API.UI;
using ReCrafted.API.UI.Controls;

namespace ReCrafted.Game
{
    public class SuperConsole : Script
    {
        /// <summary>
        /// Static instance for quick access
        /// </summary>
        public static SuperConsole Instance;

        /// <summary>
        /// Region of SuperConsole text container.
        /// </summary>
        public RectangleF TextContainerRegion => new RectangleF(0, 0, Display.Width, Display.Height / 6f);

        /// <summary>
        /// SuperConsole text container.
        /// </summary>
        public UIPanel TextContainer;

        /// <summary>
        /// Region of SuperConsole input.
        /// </summary>
        public RectangleF InputRegion => new RectangleF(0, Display.Height / 6f + 25f, Display.Width, 25);

        /// <summary>
        /// Container of console input field.
        /// </summary>
        public UIPanel InputContainer;

        /// <summary>
        /// Input text field of super console.
        /// </summary>
        public UITextField Input;

        protected internal override void OnCreate()
        {
            Instance = this;

            // Create Main Panel
            TextContainer = UIPanel.Create(TextContainerRegion, UILayoutType.Vertical, "Super Console", 10001);

            TextContainer.PanelColor = Color.Red;
            TextContainer.EnableClipping = false;
            TextContainer.EnableScrollBars = true;

            TextContainer.Layout.ReverseContainer = true;

            TextContainer.Layout.PreferredSize = new Vector2(0, 30);
            TextContainer.Layout.ForceExpandHeight = false;
            TextContainer.Layout.ForceExpandWidth = true;
            TextContainer.Layout.Padding = new UIPadding
            {
                Top = 0,
                Bottom = 5,
                Left = 5,
                Right = 10
            };
            TextContainer.Layout.Space = UIControl.DefaultFont.Size / 2f;
            TextContainer.Layout.Alignment = UILayoutAlignment.LeftBottom;
            TextContainer.Layout.Offset = new Vector2(0, -10);
            TextContainer.ApplyLayout = true;

            UIPanel inputTextFieldContainer;
            UITextField inpuTextField = new UITextField(string.Empty);

            UIPanel.CreateControl(InputRegion, ref inpuTextField, out inputTextFieldContainer);
            InputContainer = inputTextFieldContainer;
            Input = inpuTextField;

            Input.SpringAnimation = false;
            Input.MultipleLine = false;
            Input.OnSubmit += OnInputSubmit;

            Disable();
        }

        private void OnInputSubmit()
        {
            if (string.IsNullOrEmpty(Input.Text) || string.IsNullOrWhiteSpace(Input.Text))
            {
                Input.Text = string.Empty;
                return;
            }

            var text = TextContainer.Add(new UIText(string.Empty));
            text.FitRegionSizeToText = true;
            text.Text = Input.Text;
            Input.Text = string.Empty;
        }

        protected override void OnUpdate()
        {
            if (!Enabled) return;
            TextContainer.Region = TextContainerRegion;
            InputContainer.Region = InputRegion;
        }

        /// <summary>
        /// Enables super console.
        /// </summary>
        public void Enable()
        {
            TextContainer.Enabled = true;
            TextContainer.Visible = true;

            InputContainer.Enabled = true;
            InputContainer.Visible = true;

            UIControl.SetFocusedControl(Input);
        }

        /// <summary>
        /// Disable super console.
        /// </summary>
        public void Disable()
        {
            TextContainer.Enabled = false;
            TextContainer.Visible = false;

            InputContainer.Enabled = false;
            InputContainer.Visible = false;
        }

        /// <summary>
        /// Is super console enabled?
        /// </summary>
        public bool Enabled => TextContainer.Enabled;
    }
}
