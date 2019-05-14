// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using System.Linq;
using ReCrafted.API;
using ReCrafted.API.Common;
using ReCrafted.API.Content;
using ReCrafted.API.Core;
using ReCrafted.API.Core.Actors;
using ReCrafted.API.Graphics;
using ReCrafted.API.Input;

namespace ReCrafted.Game.Core
{
    public class TestScript : Script<EmptyActor>
    {
        private void Awake()
        {
            Logger.Log("TestScript Awake");
        }

        private void Update()
        {
            //Logger.Log("TestScript Update");
        }

        private void OnEnable()
        {
            Logger.Log("TestScript enabled");
        }

        private void OnDisable()
        {
            Logger.Log("TestScript disabled");
        }

        private void OnDestroy()
        {
            Logger.Log("TestScript destroyed");
        }
    }

    public class Game : GameBase<Game>
    {
        private EmptyActor _actor;

        protected override void OnInitialize()
        {
            base.OnInitialize();

            _actor = EmptyActor.Create();
            _actor.Name = "Test Actor";
            _actor.AddScript<TestScript>();
            _actor.AddChild(EmptyActor.Create());

            Logger.Log($"{_actor.Children.Length}");
            Logger.Log($"{_actor.Children.First().Position}");

            Object.DestroyNow(_actor);

            Logger.Log("Game initialized");

            ContentManager.LoadAsset<Texture>("Textures/Default", texture =>
            {
                Logger.Log("Texture loaded.");
            });
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
