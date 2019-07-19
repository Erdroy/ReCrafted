// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using System;

namespace ReCrafted.API.WebUI
{
    public sealed partial class WebUIView
    {
        public event Action BeginLoading
        {
            add => AddOnBeginLoading(value);
            remove => RemoveOnBeginLoading(value);
        }

        public event Action FinishLoading
        {
            add => AddOnFinishLoading(value);
            remove => RemoveOnFinishLoading(value);
        }

        public event Action DOMReady
        {
            add => AddOnDOMReady(value);
            remove => RemoveOnDOMReady(value);
        }
    }
}
