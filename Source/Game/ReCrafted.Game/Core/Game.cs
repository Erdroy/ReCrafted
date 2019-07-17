// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using ReCrafted.API;
using ReCrafted.API.Common;
using ReCrafted.API.Core;
using ReCrafted.API.Mathematics;
using ReCrafted.API.Physics;
using ReCrafted.API.Rendering;
using ReCrafted.API.WebUI;
using ReCrafted.Game.UI;

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
            _defaultPhysicsScene.Enabled = true;

            _camera = Object.New<CameraActor>();
            _camera.Camera.SetAsCurrent();
            _camera.Position = Vector3.BackwardLH * 10.0f;

            // Run WebUIManager
            GameSystem.AddGameSystem<WebUI>();

            // Add game panels
            WebUI.AddPanel<UIGameOverlay>();
            WebUI.AddPanel<UIGameHud>();
            WebUI.AddPanel<UIPauseMenu>();
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
