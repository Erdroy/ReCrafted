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
        private RigidBodyActor _actor1;
        private RigidBodyActor _actor2;

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

            _actor1 = RigidBodyActor.CreateStatic();
            _actor1.Position = Vector3.Up * 4.0f;
            var c = Object.New<MeshCollider>();
            c.SetMesh(new []
            {
                new Vector3(-0.5f, 0.0f, -0.5f),
                new Vector3(-0.5f, 0.0f,  0.5f),
                new Vector3( 0.5f, 0.0f,  0.5f),
                new Vector3( 0.5f, 0.0f, -0.5f),
            }, new[] { 0u, 1u, 2u, 2u, 3u, 1u });
            _actor1.AttachCollider(c);

            _actor2 = RigidBodyActor.CreateDynamic();
            _actor2.Position = Vector3.Down * 8.0f;

            _actor2.AttachCollider(Object.New<BoxCollider>());

            _cc = Object.New<CharacterActor>();
        }
        
        protected override void OnShutdown()
        {
            base.OnShutdown();
        }

        private float _wT;
        private float _sT;
        private float _aT;
        private float _dT;
        private CharacterActor _cc;

        protected override void OnUpdate()
        {
            base.OnUpdate();

            if (InputManager.IsKey(Key.W))
            {
                _wT = Time.CurrentTime;
                _actor2.AddForce(Vector3.Up * 10.0f, ForceMode.Acceleration);
            }

            if (InputManager.IsKey(Key.S))
            {
                _sT = Time.CurrentTime;
                _actor2.AddForce(Vector3.Down * 10.0f, ForceMode.Acceleration);
            }

            if (InputManager.IsKey(Key.A))
            {
                _aT = Time.CurrentTime;
                _actor2.AddForce(Vector3.Left * 10.0f, ForceMode.Acceleration);
            }

            if (InputManager.IsKey(Key.D))
            {
                _dT = Time.CurrentTime;
                _actor2.AddForce(Vector3.Right * 10.0f, ForceMode.Acceleration);
            }

            if (InputManager.IsKey(Key.E))
                _cc.Move(Vector3.ForwardLH * Time.DeltaTime);

            DebugDraw.Matrix = Matrix.RotationQuaternion(_actor2.Rotation) * Matrix.Translation(_actor2.Position);

            // Draw thrusters
            var wd = Time.CurrentTime - _wT;
            if (wd < 0.3f)
            {
                var a = 1.0f - wd / 0.3f;
                var c = new Color(Color.Yellow.ToVector3(), a);
                DebugDraw.Color = c;
                DebugDraw.DrawBox(Vector3.Down, Vector3.One * 0.25f);
            }

            var ad = Time.CurrentTime - _aT;
            if (ad < 0.3f)
            {
                var a = 1.0f - ad / 0.3f;
                var c = new Color(Color.Yellow.ToVector3(), a);
                DebugDraw.Color = c;
                DebugDraw.DrawBox(Vector3.Right, Vector3.One * 0.25f);
            }

            var dd = Time.CurrentTime - _dT;
            if (dd < 0.3f)
            {
                var a = 1.0f - dd / 0.3f;
                var c = new Color(Color.Yellow.ToVector3(), a);
                DebugDraw.Color = c;
                DebugDraw.DrawBox(Vector3.Left, Vector3.One * 0.25f);
            }

            var sd = Time.CurrentTime - _sT;
            if (sd < 0.3f)
            {
                var a = 1.0f - sd / 0.3f;
                var c = new Color(Color.Yellow.ToVector3(), a);
                DebugDraw.Color = c;
                DebugDraw.DrawBox(Vector3.Up, Vector3.One * 0.25f);
            }

            DebugDraw.Color = Color.Red;
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
