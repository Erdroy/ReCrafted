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
        public UIText DebugPanelMs;

        protected internal override void OnCreate()
        {
            var sw = Stopwatch.StartNew();
            DebugPanel = UIPanel.Create(new RectangleF(), UILayoutType.Horizontal);
            DebugPanel.ApplyLayout = false;

            DebugPanelMs = DebugPanel.Layout.AddText(string.Empty);

            var panel = UIPanel.Create(new RectangleF(200.0f, 10.0f, 200.0f, 450.0f), UILayoutType.Vertical);
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
            panel.Layout.Alignment = UILayoutAlignment.Middle;
            panel.ApplyLayout = true;

            var layout = panel.Layout;

            var button = layout.AddButton(new RectangleF(300, 300, 120, 30), "Some text!", null);
            button.SmoothColors = true;
            button.OnClick += () =>
            {
                var region = button.Region;
                region.Y += 30;
                button.Region = region;
            };

            var button2Colors = UIButtonColors.Defaults;
            button2Colors.NormalColor = Color.Orange;
            layout.AddButton(new RectangleF(300, 310, 60, 60), string.Empty, button2Colors, null);

            var dirtTexture = Texture2D.Create(Assets.ResolveAssetFilePath(AssetType.Texture, "dirt.png"));
            var boxTexture = layout.AddBox(new RectangleF(100, 100, 32, 32), dirtTexture);
            boxTexture.BoxColor = Color.Aqua;

            layout.AddBox(new RectangleF(100, 100, 32, 32), dirtTexture);
            layout.AddBox(new RectangleF(100, 105, 32, 32), dirtTexture);

            layout.AddText(new RectangleF(10, 10, 160, 30), "12345678912345678912345678912342343243");

            for (int i = 0; i < 1000; i++)
                layout.AddBox(new Color(0, 100 * i / 100, 0));

            sw.Stop();
            Logger.Write("Ui Construct Took -> " + sw.ElapsedMilliseconds + "ms");
        }

        private float _uiDebugTime;
        protected override void OnUpdate()
        {
            if (_uiDebugTime < 1)
                _uiDebugTime += (float) Time.DeltaTime;
            else
            {
                _uiDebugTime = 0f;

                DebugPanelMs.Region = new RectangleF(20, Display.Height - 40f, 130, 30);
                DebugPanelMs.Text = "Ui Process Took -> " + GameMain.TotalMilisecondsForUiPanel + "ms";
            }
        }
    }
}
