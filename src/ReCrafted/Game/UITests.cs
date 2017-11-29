// ReCrafted © 2016-2017 Always Too Late

using System.Diagnostics;
using ReCrafted.API;
using ReCrafted.API.Common;
using ReCrafted.API.Core;
using ReCrafted.API.Graphics;
using ReCrafted.API.Mathematics;
using ReCrafted.API.UI;
using ReCrafted.API.UI.Controls;

namespace ReCrafted.Game
{
    public class UITests : Script
    {
        public UIPanel DebugPanel;
        public UIText DebugPanelText;

        public UIPanel TestPanel;
        public UIBox TestBox;

        protected internal override void OnCreate()
        {
            var sw = Stopwatch.StartNew();
            DebugPanel = UIPanel.Create(new RectangleF(), UILayoutType.Vertical, "debug-panel");
            DebugPanel.ApplyLayout = false;

            DebugPanelText = DebugPanel.Add(new UIText());

            TestPanel = UIPanel.Create(new RectangleF(), UILayoutType.Vertical, "test-panel");
            TestPanel.ApplyLayout = false;

            //TestBox = TestPanel.Add(new UIBox(new RectangleF(0f, 0f, 32f, 32f)));

            var panel = UIPanel.Create(new RectangleF(200.0f, 100.0f, 200.0f, 450.0f), UILayoutType.Vertical, "panel1");
            panel.PanelColor = Color.Red;
            panel.Enabled = true;
            panel.Layout.ForceExpandHeigth = false;
            panel.Layout.ForceExpandWidth = false;
            panel.Layout.Padding = new UIPadding
            {
                Top = 5,
                Bottom = 5,
                Left = 5,
                Right = 5
            };
            panel.Layout.Space = 1f;
            panel.Layout.PreferredSize = new Vector2(30, 30);
            panel.Layout.Alignment = UILayoutAlignment.MiddleTop;
            panel.ApplyLayout = true;

            var button = panel.Add(new UIButton(new RectangleF(0, 0, 130, 32), "Some text!"));
            button.SmoothColors = true;
            button.OnClick += () =>
            {
                var region = button.Region;
                region.Y += 30;
                button.Region = region;
            };

            var toggleGroup = new UIToggleGroup();
            var toggle1 = toggleGroup.Add(panel.Add(new UIToggle(new RectangleF(0, 0, 130, 32), "TestToggle1!")));
            toggle1.TextBackgroundColor = new Color(68, 68, 68);
            var toggle2 = toggleGroup.Add(panel.Add(new UIToggle(new RectangleF(0, 0, 130, 32), "TestToggle2!")));
            toggle2.TextBackgroundColor = new Color(68, 68, 68);
            var toggle3 = toggleGroup.Add(panel.Add(new UIToggle(new RectangleF(0, 0, 130, 32), "TestToggle3!")));
            toggle3.TextBackgroundColor = new Color(68, 68, 68);

            var textField = panel.Add(new UITextField(new RectangleF(0, 0, 130, 32)));
            textField.Text = "Text Field!\nSome More Text?";

            //panel.Add(new UIText(new RectangleF(10, 10, 160, 30), "12345678912345678912345678912342343243"));

            /*for (int i = 0; i < 1000; i++)
                panel.Add(new UIBox(new Color(0, 100 * i / 100, 0)));
                */

            sw.Stop();
            Logger.Write("Ui Construct Took -> " + sw.ElapsedMilliseconds + "ms");
        }

        private float _uiDebugTime;
        protected override void OnUpdate()
        {
            if (_uiDebugTime < 0.1f)
                _uiDebugTime += (float) Time.DeltaTime;
            else
            {
                _uiDebugTime = 0f;

                DebugPanelText.Region = new RectangleF(20, Display.Height - 80f, 0, 0);
                DebugPanelText.Text = "Focused Control -> " + (UIControl.FocusedControl?.GetType().ToString() ?? "<none>") + "\n" +
                                      "Focused Control Root -> " + ((string.IsNullOrEmpty(UIControl.FocusedControl?.GetRoot()?.Name) ? UIControl.FocusedControl?.GetRoot()?.GetType().ToString() : UIControl.FocusedControl?.GetRoot()?.Name) ?? "<none>") + "\n" +
                                      "Focused Control Regon -> " + (UIControl.FocusedControl?.Region.ToString() ?? "<none>") + "\n" + 
                                      "Mouse Position -> " + Input.CursorPosition;
            }

            if (TestBox != null)
                _testBoxFollow();
        }

        private Vector2 _testBoxPosition = Vector2.One;
        private Vector2 _testBoxVelocity = Vector2.One;
        private Vector2 _testBoxTargetPosition = Vector2.One;

        private void _testBoxFollow()
        {
            _testBoxTargetPosition = Input.CursorPosition;
            UIAnimation.SpringVector2(ref _testBoxPosition, ref _testBoxVelocity, _testBoxTargetPosition, (float)Time.DeltaTime);
            TestBox.Region = new RectangleF(_testBoxPosition.X, _testBoxPosition.Y, 32, 32);
        }
    }
}
