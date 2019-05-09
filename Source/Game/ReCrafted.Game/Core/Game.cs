// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using ReCrafted.API.Common;
using ReCrafted.API.Core;
using ReCrafted.API.Core.Actors;
using ReCrafted.API.Input;
using ReCrafted.API.Mathematics;

namespace ReCrafted.Game.Core
{
    public class Game : GameBase<Game>
    {
        private EmptyActor _actor;

        protected override void OnInitialize()
        {
            base.OnInitialize();

            _actor = EmptyActor.Create();
            _actor.Name = "Test Actor";
            _actor.SetActive(true);

            var v1 = new Vector3();

            Logger.Log("Game initialized");
        }

        protected override void OnShutdown()
        {
            base.OnShutdown();

        }

        protected override void OnUpdate()
        {
            base.OnUpdate();

            if (InputManager.IsKeyDown(Key.Escape))
                Application.Quit();
        }

        protected override void OnFixedUpdate()
        {
            base.OnFixedUpdate();

        }
    }
}
