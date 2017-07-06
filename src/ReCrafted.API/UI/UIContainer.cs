// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

using System;
using System.Collections.Generic;

namespace ReCrafted.API.UI
{
    public class UIContainer : UIControl
    {
        private List<UIControl> _controls;

        /// <summary>
        /// Contains all the controls
        /// </summary>
        public UIControl[] Controls => _controls.ToArray();

        public T Add<T>() where T : UIControl
        {
            throw new NotImplementedException();
        }

        public void Remove<T>(T instance) where T : UIControl
        {
            throw new NotImplementedException();
        }

        public override void Draw()
        {
            throw new NotImplementedException();
        }
    }
}
