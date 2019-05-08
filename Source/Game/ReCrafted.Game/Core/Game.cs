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
            base.OnInitialize();

            Logger.Log("Game initialized");

            var actor = EmptyActor.Create();
        }

        protected override void OnShutdown()
        {
            base.OnShutdown();

        }

        protected override void OnUpdate()
        {
            base.OnUpdate();

        }

        protected override void OnFixedUpdate()
        {
            base.OnFixedUpdate();

        }
    }
}
