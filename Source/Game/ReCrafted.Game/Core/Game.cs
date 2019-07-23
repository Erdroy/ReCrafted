// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using ReCrafted.API;
using ReCrafted.API.Common;
using ReCrafted.API.Content;
using ReCrafted.API.Core;
using ReCrafted.API.Input;
using ReCrafted.API.Mathematics;
using ReCrafted.API.Physics;
using ReCrafted.API.Rendering.Debug;
using ReCrafted.API.Voxels;
using ReCrafted.API.WebUI;
using ReCrafted.Game.Player;
using ReCrafted.Game.UI;

namespace ReCrafted.Game.Core
{
    public class Game : GameBase<Game>
    {
        private PhysicsScene _defaultPhysicsScene;
        private RigidBodyActor _floor;

        private VoxelObjectAsset _moon;

        public PlayerManager CurrentPlayer;

        protected override void OnInitialize()
        {
            base.OnInitialize();

            Logger.Log("Game initialized");

            // Temporary, create default scene.
            // Scenes will be handled internally when physics will get clustering system.
            _defaultPhysicsScene = PhysicsManager.CreateScene();
            _defaultPhysicsScene.Enabled = true;

            InputManager.ShowCursor = false;
            InputManager.LockCursor = true;

            // Spawn player
            CurrentPlayer = PlayerManager.SpawnPlayer(new Vector3(0.0f, 14.0f, 0.0f), Quaternion.Identity);

            _floor = RigidBodyActor.CreateStatic();
            var collider = Object.New<SphereCollider>();
            collider.Radius = 10.0f;
            _floor.AttachCollider(collider);

            // Run WebUIManager
            GameSystem.AddGameSystem<WebUI>();

            // Add game panels
            WebUI.AddPanel<UIGameOverlay>();
            WebUI.AddPanel<UIGameHud>();
            WebUI.AddPanel<UIPauseMenu>();

            // Create test planet
            _moon = ContentManager.LoadAsset<VoxelObjectAsset>("Voxels/Objects/Moon");
        }
        
        protected override void OnShutdown()
        {
            base.OnShutdown();
        }

        protected override void OnUpdate()
        {
            base.OnUpdate();

            DebugDraw.Color = Color4.White;
            DebugDraw.DrawWireSphere(Vector3.Zero, 10.0f);
        }

        protected override void OnFixedUpdate()
        {
            base.OnFixedUpdate();

        }
    }
}
