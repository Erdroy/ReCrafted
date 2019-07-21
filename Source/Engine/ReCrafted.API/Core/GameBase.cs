// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using System;
using ReCrafted.API.Common;
using ReCrafted.Tools.Common;

namespace ReCrafted.API.Core
{
    /// <summary>
    ///     GameBase class. Base class for game's main class.
    /// </summary>
    public abstract class GameBase<TGame>
    {
        protected virtual void OnInitialize()
        {
            Logger.Log($"Game startup ({DateTime.Now})");

            // Load GameInfo
            GameInfo.Current = GameInfo.FromFile("../GameInfo.json");

            // Register unhandled exception handler
            Exceptions.RegisterUEHandler();
        }

        protected virtual void OnShutdown()
        {

        }

        protected virtual void OnUpdate()
        {
            // Update all game systems
            GameSystem.UpdateAll();

        }

        protected virtual void OnFixedUpdate()
        {

        }

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
