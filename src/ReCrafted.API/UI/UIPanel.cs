// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

using System;
using System.Collections.Generic;

namespace ReCrafted.API.UI
{
    public class UIPanel : UIControl
    {
        /// <summary>
        /// Contains all the controls
        /// </summary>
        public UILayout Layout { get; }

        /// <summary>
        /// Pokazanie/ukrycie panelu.
        /// </summary>
        public bool Visible { get; set; }

        private static List<UIPanel> _panels;

        public static UIPanel Create()
        {
            throw new NotImplementedException();
        }

        public override void Draw()
        {
            throw new NotImplementedException();
        }

        internal static void DrawAll()
        {
            throw new NotImplementedException();
        }
    }
}
