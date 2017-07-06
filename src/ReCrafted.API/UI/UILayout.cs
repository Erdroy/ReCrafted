// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

using System;
using ReCrafted.API.Core;

namespace ReCrafted.API.UI
{
    public enum UILayoutType
    {
        Grid,
        FixedGrid,
        Horizontal,
        Vertical
    }

    public class UILayout : UIContainer
    {
        public UILayoutType Type { get; set; }
        public Vector2 PreferedSize { get; set; }

        /// <summary>
        /// Recalculate Layout, duh
        /// </summary>
        public void Recalculate()
        {
            throw new NullReferenceException();
        }
    }
}
