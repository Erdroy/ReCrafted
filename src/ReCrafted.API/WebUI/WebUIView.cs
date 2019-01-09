// ReCrafted (c) 2016-2019 Always Too Late

using System;
using ReCrafted.API.Graphics;

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

        /// <summary>
        /// Calls JavaScript function of specified name with given parameters.
        /// </summary>
        /// <param name="functionName">The JavaScript global function.</param>
        /// <param name="parameters">The function parameters.</param>
        public void Call(string functionName, params object[] parameters)
        {
            Internal_Call(NativePtr, functionName, IntPtr.Zero, parameters);
        }

        /// <summary>
        /// Calls JavaScript function of specified name with given parameters and return type.
        /// </summary>
        /// <param name="functionName">The JavaScript global function.</param>
        /// <param name="parameters">The function parameters.</param>
        public TReturn Call<TReturn>(string functionName, params object[] parameters)
        {
            return (TReturn)Internal_Call(NativePtr, functionName, typeof(TReturn).TypeHandle.Value, parameters);
        }
        
        /// <summary>
        /// Creates new view and loads page from given uri.
        /// </summary>
        /// <param name="uri">The URI that will be used to load page from.</param>
        /// <param name="onReady">Called on DOMReady.</param>
        /// <param name="onLoaded">Called on FinishLoading.</param>
        /// <returns>The newly created WebUIView.</returns>
        public static WebUIView Create(string uri, ViewCallback onReady, ViewCallback onLoaded = null)
        {
            var view = Create();

            view.Navigate(uri);

            view.DOMReady += onReady;

            if (onLoaded != null)
                view.DOMReady += onLoaded;

            return view;
        }

        /// <summary>
        /// Called when page is starting to load, immediately after Navigate.
        /// </summary>
        public event ViewCallback BeginLoading;

        /// <summary>
        /// Called when page loading is done.
        /// Usually at the same frame that the page is being displayed first time.
        /// </summary>
        public event ViewCallback FinishLoading;

        /// <summary>
        /// Called when DOM is ready while loading. Use for JavaScript bindings etc.
        /// </summary>
        public event ViewCallback DOMReady;

        /// <summary>
        /// The render texture of this view.
        /// Only valid when not fullscreen.
        /// </summary>
        public Texture RenderTexture => null;
    }
}
