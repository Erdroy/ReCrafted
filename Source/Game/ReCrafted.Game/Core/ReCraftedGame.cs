// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using ReCrafted.API.Common;
using ReCrafted.API.Core;

namespace ReCrafted.Game.Core
{
    public class ReCraftedGame : GameBase<ReCraftedGame>
    {
        protected override void OnInitialize()
        {
            Logger.Log("Hello from C#!");

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

        protected override void OnRender()
        {
        }
    }
}
