// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using System;

namespace ReCrafted.API.Core
{
    /// <summary>
    ///     GameBase class. Base class for game's main class.
    /// </summary>
    public abstract class GameBase<TGame>
    {
        protected abstract void OnInitialize();
        protected abstract void OnShutdown();
        protected abstract void OnUpdate();
        protected abstract void OnFixedUpdate();
        protected abstract void OnRender();

        public void Quit()
        {
            throw new NotImplementedException();
        }

        public void Pause(bool pause)
        {
            throw new NotImplementedException();
        }
    }
}
