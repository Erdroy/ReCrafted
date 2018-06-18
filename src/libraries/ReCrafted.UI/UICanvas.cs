// ReCrafted (c) 2016-2018 Always Too Late

using System;

namespace ReCrafted.UI
{
    public class UICanvas : IDisposable
    {
        private UIControl _rootControl;

        public UICanvas()
        {
            _rootControl = new UIControl();
        }

        public void Update()
        {
            _rootControl.Update();
        }

        public void Dispose()
        {
            _rootControl?.Dispose();
        }
    }
}
