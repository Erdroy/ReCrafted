// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using ReCrafted.API.Common;
using ReCrafted.API.Core;
using ReCrafted.API.Core.Actors;
using ReCrafted.API.Mathematics;

namespace ReCrafted.Game.Core
{
    public class Game : GameBase<Game>
    {
        protected override void OnInitialize()
        {
            Logger.Log("Game initialized");

            // Register unhandled exception handler
            Exceptions.RegisterUEHandler();
        }

        protected override void OnShutdown()
        {
        }

        protected override void OnUpdate()
        {
        }

        protected override void OnFixedUpdate()
        {
        }
    }
}
