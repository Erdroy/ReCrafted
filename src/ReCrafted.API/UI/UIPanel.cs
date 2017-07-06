using System;
using System.Collections.Generic;

namespace ReCrafted.API.UI
{
    public class UIPanel : UIControl
    {
        /// <summary>
        /// Zawiera wszystkie kontrolki.
        /// </summary>
        public UILayout Layout { get; }

        /// <summary>
        /// Pokazanie/ukrycie panelu.
        /// </summary>
        public bool Visible { get; set; }

        static List<UIPanel> _panels;
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
