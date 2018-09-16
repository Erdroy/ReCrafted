// ReCrafted Editor (c) 2016-2018 Always Too Late

using System;

namespace ReCrafted.Editor.Core
{
    /// <summary>
    /// Application base class.
    /// </summary>
    public abstract class ApplicationBase : IDisposable
    {
        /// <summary>
        /// Default constructor.
        /// </summary>
        protected ApplicationBase()
        {
            Current = this;
        }

        /// <summary>
        /// Starts the application.
        /// </summary>
        internal void Run()
        {
            IsRunning = true;

            OnInit();
            OnLoad();

            while (IsRunning)
            {
                OnUpdate();
            }

            OnUnload();
        }

        /// <summary>
        /// Disposes the application.
        /// </summary>
        public void Dispose()
        {
            Current = null;
            IsRunning = false;
        }

        /// <summary>
        /// Shutdowns the application.
        /// </summary>
        public void Shutdown()
        {
            Dispose();
        }

        protected abstract void OnInit();
        protected abstract void OnLoad();
        protected abstract void OnUnload();
        protected abstract void OnUpdate();

        /// <summary>
        /// Is the application running?
        /// </summary>
        public bool IsRunning { get; set; }

        /// <summary>
        /// The current application instance.
        /// </summary>
        public static ApplicationBase Current { get; private set; }
    }
}
