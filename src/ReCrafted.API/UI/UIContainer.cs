using System;
using System.Collections.Generic;

namespace ReCrafted.API.UI
{
    public class UIContainer : UIControl
    {
        List<UIControl> _controls;
        /// <summary>
        /// Zawiera wszystkie kontrolki.
        /// </summary>
        UIControl[] Controls { get { return _controls.ToArray(); } }
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
