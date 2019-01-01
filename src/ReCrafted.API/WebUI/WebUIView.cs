// ReCrafted (c) 2016-2019 Always Too Late

namespace ReCrafted.API.WebUI
{
    public delegate void ViewCallback();

    public partial class WebUIView
    {
        protected virtual void OnBeginLoading()
        {
            BeginLoading?.Invoke();
        }

        protected virtual void OnFinishLoading()
        {
            FinishLoading?.Invoke();
        }

        protected virtual void OnDOMReady()
        {
            DOMReady?.Invoke();
        }

        public event ViewCallback BeginLoading;
        public event ViewCallback FinishLoading;
        public event ViewCallback DOMReady;
    }
}
