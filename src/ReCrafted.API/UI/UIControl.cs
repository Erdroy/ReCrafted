// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

using ReCrafted.API.Core;

namespace ReCrafted.API.UI
{
    public abstract class UIControl
    {
        public UIControl Parent { get; internal set; }
        public RectangleF Region { get; set; }
        public bool Enabled { get; set; }

        public abstract void Draw();
    }
}
