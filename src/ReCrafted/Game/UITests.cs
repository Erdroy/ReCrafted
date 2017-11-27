// ReCrafted © 2016-2017 Always Too Late

using ReCrafted.API;
using ReCrafted.API.Common;
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

        public UIPanel Panel;
        public UIPanel Panel2;

        protected internal override void OnCreate()
        {
            DebugPanel = UIPanel.Create(new RectangleF(), UILayoutType.Horizontal);
            DebugPanel.ApplyLayout = false;

            DebugPanelMs = DebugPanel.Layout.Add(UIText.Create(string.Empty));

            Panel = UIPanel.Create(new RectangleF(10.0f, 10.0f, 200.0f, 450.0f), UILayoutType.Vertical);
            Panel.PanelColor = Color.Red;
            Panel.Visible = false;
            Panel.Layout.ForceExpandHeigth = false;
            Panel.Layout.ForceExpandWidth = false;
            Panel.Layout.Padding = new UIPadding
            {
                Top = 5,
                Bottom = 5,
                Left = 5,
                Right = 5
            };
            Panel.Layout.Space = 0f;
            Panel.Layout.PreferredSize = new Vector2(0, 30);
            Panel.ApplyLayout = false;

            var button = Panel.Layout.Add(UIButton.Create(new RectangleF(300, 300, 120, 30), "Some text!"));
            button.SmoothColors = true;
            button.OnClick += () =>
            {
                var region = button.Region;
                region.Y += 30;
                button.Region = region;
            };

            var button2 = Panel.Layout.Add(UIButton.Create(new RectangleF(300, 310, 60, 60)));
            var button2Colors = UIButtonColors.Defaults;
            button2Colors.NormalColor = Color.Orange;
            button2.Colors = button2Colors;

            var boxTexture = Panel.Layout.Add(UIBox.Create(new RectangleF(100, 100, 32, 32), Texture2D.Create(Assets.ResolveAssetFilePath(AssetType.Texture, "dirt.png"))));
            boxTexture.BoxColor = Color.Aqua;

            Panel.Layout.Add(UIBox.Create(new RectangleF(100, 100, 32, 32), Texture2D.Create(Assets.ResolveAssetFilePath(AssetType.Texture, "dirt.png"))));
            Panel.Layout.Add(UIBox.Create(new RectangleF(100, 100, 32, 32), Texture2D.Create(Assets.ResolveAssetFilePath(AssetType.Texture, "dirt.png"))));
            Panel.Layout.Add(UIBox.Create(new RectangleF(100, 100, 32, 32), Texture2D.Create(Assets.ResolveAssetFilePath(AssetType.Texture, "dirt.png"))));
            Panel.Layout.Add(UIBox.Create(new RectangleF(100, 100, 32, 32), Texture2D.Create(Assets.ResolveAssetFilePath(AssetType.Texture, "dirt.png"))));
            Panel.Layout.Add(UIBox.Create(new RectangleF(100, 100, 32, 32), Texture2D.Create(Assets.ResolveAssetFilePath(AssetType.Texture, "dirt.png"))));
            Panel.Layout.Add(UIBox.Create(new RectangleF(100, 100, 32, 32), Texture2D.Create(Assets.ResolveAssetFilePath(AssetType.Texture, "dirt.png"))));
            Panel.Layout.Add(UIBox.Create(new RectangleF(100, 100, 32, 32), Texture2D.Create(Assets.ResolveAssetFilePath(AssetType.Texture, "dirt.png"))));
            Panel.Layout.Add(UIBox.Create(new RectangleF(100, 100, 32, 32), Texture2D.Create(Assets.ResolveAssetFilePath(AssetType.Texture, "dirt.png"))));
            Panel.Layout.Add(UIBox.Create(new RectangleF(100, 100, 32, 32), Texture2D.Create(Assets.ResolveAssetFilePath(AssetType.Texture, "dirt.png"))));

            //var text = Panel.Layout.Add(UIText.Create(new RectangleF(10, 10, 160, 30), "12345678912345678912345678912342343243"));

            /*
            for (int i = 0; i < 1000; i++)
            {
                var layout = Panel.Layout.Add(UIBox.Create());
                layout.BoxColor = new Color(0, 100 * i / 100, 0);
            } 
            */

            Panel2 = UIPanel.Create(new RectangleF(10.0f, 10.0f, 200.0f, 450.0f), UILayoutType.Vertical);
            Panel2.PanelColor = Color.Red;
            Panel2.Visible = false;
            Panel2.Layout.ForceExpandHeigth = false;
            Panel2.Layout.ForceExpandWidth = false;
            Panel2.Layout.Padding = new UIPadding
            {
                Top = 5,
                Bottom = 5,
                Left = 5,
                Right = 5
            };
            Panel2.Layout.Space = 0f;
            Panel2.Layout.PreferredSize = new Vector2(0, 30);
            Panel2.ApplyLayout = false;

            var p2Button1 = Panel2.Layout.Add(UIButton.Create(new RectangleF(100, 110, 32, 32), "Lol"));
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
                DebugPanelMs.Text = "Ui Process Took -> " + GameMain.TotalMilisecondsForUiPanel;
            }
        }
    }
}
