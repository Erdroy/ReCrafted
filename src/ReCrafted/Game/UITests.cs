// ReCrafted © 2016-2017 Always Too Late

using ReCrafted.API.Common;
using ReCrafted.API.Core;
using ReCrafted.API.Mathematics;
using ReCrafted.API.UI;
using ReCrafted.API.UI.Controls;

namespace ReCrafted.Game
{
    public class UITests : Script
    {
        public UIPanel Panel;

        protected internal override void OnCreate()
        {
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

            var button = Panel.Layout.Add(UIButton.Create(new RectangleF(300, 300, 120, 30)));
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

            /*
            for (int i = 0; i < 1000; i++)
            {
                var layout = Panel.Layout.Add(UIBox.Create());
                layout.BoxColor = new Color(0, 100 * i / 100, 0);
            } 
            */
        }
    }
}
