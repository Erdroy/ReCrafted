// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

using System;
using ReCrafted.API.Mathematics;

namespace ReCrafted.API.UI
{
    /// <summary>
    /// UILayoutType enum
    /// </summary>
    public enum UILayoutType
    {
        Grid,
        FixedGrid,
        Horizontal,
        Vertical
    }

    /// <summary>
    /// UILayout class, implements lay-outing system.
    /// </summary>
    public class UILayout : UIContainer
    {
        /// <summary>
        /// Recalculates Layout, duh.
        /// </summary>
        public void Recalculate()
        {
            throw new NullReferenceException();
        }

        /// <summary>
        /// The layout type.
        /// </summary>
        public UILayoutType Type { get; set; }

        /// <summary>
        /// Preferred size for Grid and H/V layout type.
        /// </summary>
        public Vector2 PreferredSize { get; set; }
    }
}
