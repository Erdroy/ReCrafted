// ReCrafted © 2016-2017 Always Too Late

using System;
using System.Collections.Generic;
using System.Diagnostics;
using ReCrafted.API.Common;
using ReCrafted.API.Core;
using ReCrafted.API.Mathematics;
using ReCrafted.API.UI;
using ReCrafted.API.UI.Controls;

namespace ReCrafted.Game
{
    public class UITests : Script
    {
        public UIPanel DebugPanel;
        public UIText DebugPanelText;
        protected internal override void OnCreate()
        {
            var sw = Stopwatch.StartNew();

            // ui debug
            DebugPanel = UIPanel.Create(new RectangleF(), UILayoutType.Vertical, "debug-panel");
            DebugPanel.ApplyLayout = false;

            DebugPanelText = DebugPanel.Add(new UIText());

            // test panel
            /*
            var panel = UIPanel.Create(new RectangleF(10.0f, 250.0f, 200.0f, 450.0f), UILayoutType.Vertical, "panel1");
            panel.PanelColor = Color.Red;
            panel.Enabled = true;
            panel.Layout.ForceExpandHeight = false;
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

            var panelButton = panel.Add(new UIButton(new RectangleF(0, 0, 130, 32), "Some text!"));
            panelButton.SmoothColors = true;
            panelButton.OnClick += () =>
            {
                var region = panelButton.Region;
                region.Y += 30;
                panelButton.Region = region;
            };

            var panelToggleGroup = new UIToggleGroup(1, 2);
            var panelToggle1 = panelToggleGroup.Add(panel.Add(new UIToggle(new RectangleF(0, 0, 130, 32), "TestToggle1!")));
            panelToggle1.TextBackgroundColor = new Color(68, 68, 68);
            var panelToggle2 = panelToggleGroup.Add(panel.Add(new UIToggle(new RectangleF(0, 0, 130, 32), "TestToggle2!")));
            panelToggle2.TextBackgroundColor = new Color(68, 68, 68);
            var panelToggle3 = panelToggleGroup.Add(panel.Add(new UIToggle(new RectangleF(0, 0, 130, 32), "TestToggle3!")));
            panelToggle3.TextBackgroundColor = new Color(68, 68, 68);

            var panelTextField = UIControl.CreateControl(new UITextField(new RectangleF(300, 300, 130, 128), string.Empty));
            panelTextField.Text = "123\n456\n789\n\nqwe\n\n\nrty";
            *

            /*
            var freeScrollbarText = UIControl.CreateControl(new UIText(new RectangleF(100, 530, 100, 30)));
            var freeScrollBar = UIControl.CreateControl(new UIScrollbar(new RectangleF(200, 500, 150, 450)) { Vertical = true, Size = 0.1f });

            freeScrollBar.OnValueChanged += value =>
            {
                freeScrollbarText.Text = freeScrollBar.Position.ToString(CultureInfo.InvariantCulture) + '\n' + freeScrollBar.Size.ToString(CultureInfo.InvariantCulture);
            };

            var freeScrollBarButton = new UIButton (new RectangleF(100, 500, 100, 30)) { Text = "Click Me!" };
            freeScrollBarButton.OnClick += () =>
            {
                var r = new Random();
                freeScrollBar.Position = r.Next(0, 10) / 10f;
                freeScrollBar.Size = r.Next(0, 10) / 10f;
            };

            UIControl.CreateControl(ref freeScrollBarButton);      
            */

            //var dropDown = UIControl.CreateControl(new UIDropdown(new RectangleF(600, 600, 120, 30)));
            //var values = new List<string> {"Hello", "World", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12"};
            //dropDown.Values = values;
            //dropDown.ListHeight = 150;
            
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

                DebugPanelText.Region = new RectangleF(20, Display.Height - 100f, 0, 0);
                DebugPanelText.Text = "Focused Control -> " + (UIControl.FocusedControl?.GetType().ToString() ?? "<none>") + " (" + (UIControl.FocusedControl?.Name ?? "<none>") + ") \n" +
                                      "Focused Control Root -> " + ((string.IsNullOrEmpty(UIControl.FocusedControl?.GetRoot()?.Name) ? UIControl.FocusedControl?.GetRoot()?.GetType().ToString() : UIControl.FocusedControl?.GetRoot()?.Name) ?? "<none>") + "\n" +
                                      "Focused Control Region -> " + (UIControl.FocusedControl?.Region.ToString() ?? "<none>") + "\n" +
                                      "Under Mouse Control -> " + (UIPanel.Collision?.GetType().ToString() ?? "<null>") + " (" + (UIPanel.Collision?.Name ?? "<null>") + ") \n" +
                                      "Mouse Position -> " + Input.CursorPosition;
            }
        }
    }
}
