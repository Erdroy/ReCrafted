// ReCrafted (c) 2016-2019 Always Too Late

using ReCrafted.API.Common;
using ReCrafted.API.Core;

namespace ReCrafted.API.WebUI
{
    /// <summary>
    /// WebUIPanel base class.
    /// </summary>
    public abstract class WebUIPanel
    {
        private readonly double _createTime;

        /// <summary>
        /// Creates new WebUIPanel using given html file.
        /// </summary>
        /// <param name="htmlFile">The html file that will be loaded.</param>
        /// <param name="activeOnStart">Should the view be activated on start?</param>
        protected WebUIPanel(string htmlFile, bool activeOnStart = true)
        {
            HtmlFile = htmlFile;

            _createTime = Time.CurrentTime;

            // Create view
            View = WebUIView.Create();
            View.Active = activeOnStart;
            View.BeginLoading += OnBeginLoading;
            View.FinishLoading += OnFinishLoading;

            // Refresh to start loading the html file
            Refresh();
        }
        
        /// <summary>
        /// Reloads the entire view.
        /// Equal to `View.Navigate(View.HtmlFile)`.
        /// </summary>
        public void Refresh()
        {
            View.Navigate(HtmlFile);
        }

        /// <summary>
        /// Shows the view.
        /// </summary>
        public void Show()
        {
            View.Active = true;
        }

        /// <summary>
        /// Hides the view.
        /// </summary>
        public void Hide()
        {
            View.Active = false;
        }

        private void OnBeginLoading()
        {
            IsLoaded = false;

            OnViewBeginLoading();
            RegisterBindings();
        }

        private void OnFinishLoading()
        {
            IsLoaded = true;

            // Execute `OnViewFinishLoading` event
            OnViewFinishLoading();

            // Say something at the end
            Logger.Log($"WebUIPanel '{this}' loaded in {Time.CurrentTime - _createTime:f2} ms.");
        }

        internal void Create()
        {
            OnCreate();
        }

        internal void Destroy()
        {
            View.Destroy();
            OnDestroy();
        }

        internal void Update()
        {
            OnUpdate();
        }
        
        protected virtual void OnCreate() { }
        protected virtual void OnDestroy() { }
        protected virtual void OnUpdate() { }

        protected virtual void RegisterBindings() { }
        protected virtual void OnViewBeginLoading() { }
        protected virtual void OnViewFinishLoading() { }

        /// <summary>
        /// The source HTML file that is being used by this panel.
        /// </summary>
        public string HtmlFile { get; }

        /// <summary>
        /// True when loaded and able to display.
        /// </summary>
        public bool IsLoaded { get; private set; }

        /// <summary>
        /// The WebUIView instance that is being used by this panel.
        /// </summary>
        protected WebUIView View { get; }
    }
}
