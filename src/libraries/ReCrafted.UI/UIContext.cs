// ReCrafted (c) 2016-2018 Always Too Late

using System;

namespace ReCrafted.UI
{
    public class UIContext : IDisposable
    {
        private UIContext() { }

        public UIContext(UIRendererBase renderer, UIImageLoaderBase imageLoader)
        {
            if(Current != null)
                throw new Exception("Cannot create more than one UIContext!");

            Current = this;

            Renderer = renderer;
            ImageLoader = imageLoader;
        }

        public void Update()
        {
            Renderer.Update();
        }

        public void Dispose()
        {
            Renderer?.Dispose();
            ImageLoader?.Dispose();
        }

        public UIRendererBase Renderer { get; }

        public UIImageLoaderBase ImageLoader { get; }

        public static UIContext Current { get; private set; }
    }
}
