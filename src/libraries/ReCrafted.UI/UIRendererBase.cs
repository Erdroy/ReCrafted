// ReCrafted (c) 2016-2018 Always Too Late

using System;
using System.Collections.Generic;

namespace ReCrafted.UI
{
    public class UIRendererBase : IDisposable
    {
        private readonly List<UICanvas> _canvases = new List<UICanvas>();
        
        internal void Update()
        {
            foreach (var canvas in _canvases)
                canvas.Update();
        }

        public void Dispose()
        {
            foreach (var canvas in _canvases)
                canvas?.Dispose();
        }

        public UICanvas CreateCanvas()
        {
            var canvas = new UICanvas();
            _canvases.Add(canvas);
            return canvas;
        }
        
        public IReadOnlyList<UICanvas> CanvasList => _canvases;
    }
}
