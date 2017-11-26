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
            Panel.Enabled = true;
            Panel.Visible = true;
            Panel.PanelColor = Color.Red;
            Panel.Layout.ForceExpandHeigth = true;
            Panel.Layout.ForceExpandWidth = true;
            Panel.Layout.Padding = new UIPadding
            {
                Top = 5,
                Bottom = 5,
                Left = 5,
                Right = 5
            };
            Panel.Layout.Space = 25f;
            Panel.Layout.PreferredSize = new Vector2(0, 30);

            var box1 = Panel.Layout.Add(new UIBox());
            box1.BoxColor = Color.Aqua;
            box1.Region = new RectangleF(10.0f, 10.0f, 230.0f, 0);

            var box2 = Panel.Layout.Add(new UIBox());
            box2.BoxColor = Color.Green;
            box2.Region = new RectangleF(10.0f, 120.0f, 210.0f, 200.0f);

            var box3 = Panel.Layout.Add(new UIBox());
            box3.BoxColor = Color.Yellow;
            box3.Region = new RectangleF(10.0f, 220.0f, 190.0f, 0);

            /*
            for (int i = 0; i < 1000; i++)
            {
                var box = Panel.Layout.Add(new UIBox());
                box.BoxColor = new Color(0, 100 * i / 100, 0);
            }
            */
        }

        private double _time;
        private bool _back;
        protected override void OnUpdate()
        {
            if (Panel == null) return;
            if (!Input.IsKey(Keys.F)) return;
            _time += Time.DeltaTime * 8f;
            if (_time > 10f)
            {
                _time = 0f;
                _back = !_back;
            }

            var newRegion = Panel.Region;
            var time = (float)Time.DeltaTime * 200f;
            if (!_back)
            {
                newRegion.Height += time;
            }
            else
            {
                newRegion.Height -= time;

            }
            Panel.Region = newRegion;
        }
    }
}
