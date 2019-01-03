// ReCrafted (c) 2016-2019 Always Too Late

using System;
using ReCrafted.API;
using ReCrafted.API.Core;
using ReCrafted.API.Graphics;
using ReCrafted.API.Input;
using ReCrafted.API.Mathematics;
using ReCrafted.API.Physics;
using ReCrafted.API.WebUI;
using ReCrafted.Common;
using ReCrafted.Game.Player;

namespace ReCrafted.Game
{
    internal class GameMain : Application
    {
        private WebUIView _uiGameHud;
        private WebUIView _uiGameOverlay;

        public static PlayerManager CurrentPlayer { get; private set; }

        // initialize
        protected override void Initialize()
        {
            try
            {
                Instance = this;

                // register unhandled exception handler
                Exceptions.RegisterUEHandler();
                
                Cursor.Show = false;
                Cursor.Lock = true;

                // Spawn player
                CurrentPlayer = PlayerManager.SpawnPlayer(new Vector3(25.0f, 1100.0f, 60.0f), Quaternion.Identity);

                // load game info
                GameInfo.FromFile(Assets.ResolveAssetFilePath("gameinfo.json"));
                
                // apply target fps
                TargetFps = 60;
                
                _uiGameOverlay = WebUIView.Create("file:///game/overlay.html", () =>
                {
                    Logger.Log("DOMReady");
                });

                _uiGameHud = WebUIView.Create("file:///game/hud.html", () =>
                {
                    Logger.Log("DOMReady");
                });
            }
            catch (Exception exception)
            {
                Exceptions.WriteException(exception);
            }
        }

        // on update
        protected override void Update()
        {
            try
            {
                // Update game systems
                GameSystem.UpdateAll();

                if (InputManager.IsKeyDown(Key.F5))
                {
                    _uiGameHud.Navigate("file:///game/hud.html");
                    _uiGameOverlay.Navigate("file:///game/overlay.html");
                }
                
                if (InputManager.IsKeyDown(Key.Escape))
                {
                    Cursor.Show = !Cursor.Show;
                    Cursor.Lock = !Cursor.Show;
                }
                
                if (PhysicsManager.RayCast(Camera.Current.Position, Camera.Current.Forward, out var hit, 10.0f, 1))
                {
                    DebugDraw.Matrix = Matrix.Identity;
                    DebugDraw.Color = new Color(0xFF1000FF);
                    DebugDraw.DrawWireSphere(hit.Point, 1.5f * 0.5f);
                }

                // draw world-space lines
                DebugDraw.Color = new Color(0, 255, 0, 32);
                DebugDraw.DrawLine(Vector3.Down * 2000.0f, Vector3.Up * 2000.0f);
                DebugDraw.Color = new Color(255, 0, 0, 32);
                DebugDraw.DrawLine(Vector3.Left * 2000.0f, Vector3.Right * 2000.0f);
                DebugDraw.Color = new Color(0, 0, 255, 32);
                DebugDraw.DrawLine(Vector3.ForwardLH * 2000.0f, Vector3.BackwardLH * 2000.0f);
            }
            catch (Exception exception)
            {
                Exceptions.WriteException(exception);
            }
        }

        // simulate
        protected override void Simulate()
        {
        }

        // draw
        protected override void Render()
        {
        }

        // draw ui
        protected override void RenderUI()
        {
        }

        // shutdown
        protected override void Shutdown()
        {
        }
        
        /// <summary>
        /// Instance of main game class.
        /// </summary>
        public static GameMain Instance { get; private set; }
    }
}