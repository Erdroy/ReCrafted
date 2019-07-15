// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using ReCrafted.API;
using ReCrafted.API.Common;
using ReCrafted.API.Core;
using ReCrafted.API.Input;
using ReCrafted.API.Mathematics;
using ReCrafted.API.Physics;
using ReCrafted.API.Rendering;
using ReCrafted.API.Rendering.Debug;

namespace ReCrafted.Game.Core
{
    public class Game : GameBase<Game>
    {
        private CameraActor _camera;
        private PhysicsScene _defaultPhysicsScene;

        protected override void OnInitialize()
        {
            base.OnInitialize();

            Logger.Log("Game initialized");

            // Temporary, create default scene.
            // Scenes will be handled internally when physics will get clustering system.
            _defaultPhysicsScene = PhysicsManager.CreateScene();

            _camera = Object.New<CameraActor>();
            _camera.Position = Vector3.BackwardLH * 10.0f;
        }

        protected override void OnShutdown()
        {
            base.OnShutdown();
        }

        protected override void OnUpdate()
        {
            base.OnUpdate();

            DebugDraw.Matrix = Matrix.RotationYawPitchRoll(Time.CurrentTime * 0.5f, Time.CurrentTime * 0.5f, Time.CurrentTime * 0.5f) * Matrix.Translation(_camera.Transform.Translation + _camera.Transform.Forward * 10.0f);
            DebugDraw.DrawBox(Vector3.Zero, Vector3.One);

            if (InputManager.IsKeyDown(Key.Escape))
                Application.Quit();
        }

        protected override void OnFixedUpdate()
        {
            base.OnFixedUpdate();

        }
    }
}
