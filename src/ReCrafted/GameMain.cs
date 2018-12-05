// ReCrafted (c) 2016-2018 Always Too Late

using System;
using ReCrafted.API;
using ReCrafted.API.Common;
using ReCrafted.API.Core;
using ReCrafted.API.Graphics;
using ReCrafted.API.Mathematics;
using ReCrafted.API.Physics;
using ReCrafted.API.UI;
using ReCrafted.API.UI.Controls;
using ReCrafted.Common;
using ReCrafted.Game.Interface;
using ReCrafted.Game.Player;

namespace ReCrafted.Game
{
    internal class GameMain : Application
    {
        // cross hair control
        private UIBox _crosshairBox;

        // build number control
        private UIText _buildNumberText;

        private WebUIView _webView;

        public static PlayerManager CurrentPlayer { get; private set; }

        // initialize
        protected override void Initialize()
        {
            try
            {
                Instance = this;

                // register unhandled exception handler
                Exceptions.RegisterUEHandler();
                
                // initialize default ui panel
                UIControl.Init();

                Cursor.Show = false;
                Cursor.Lock = true;

                // Spawn player
                CurrentPlayer = PlayerManager.SpawnPlayer(new Vector3(25.0f, 1100.0f, 60.0f), Quaternion.Identity);

                // load game info
                GameInfo.FromFile(Assets.ResolveAssetFilePath("gameinfo.json"));
                
                // apply target fps
                TargetFps = 60;

                // create some default controls
                _crosshairBox =
                    UIControl.CreateControl(
                        new UIBox(Sprite.Create(Assets.ResolveAssetFilePath(AssetType.Interface, "crosshair.png"))));

                _buildNumberText = UIControl.CreateControl(new UIText());
                _buildNumberText.Text =
                    "ReCrafted " + GameInfo.Current.BuildName + " build " + GameInfo.Current.BuildNumber;

                // Initialize default game systems
                GameSystem.AddGameSystem<PauseMenu>();
                GameSystem.AddGameSystem<Messenger>();

                _webView = WebUI.Create();
                _webView.Navigate("https://google.com/");
                //_webView.Navigate("http://evanw.github.io/csg.js/");
                //_webView.Navigate("file:///menu/menu.html");
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
                
                _crosshairBox.Region =
                    new RectangleF(Display.Width / 2.0f - 8.0f, Display.Height / 2.0f - 8.0f, 16.0f, 16.0f);
                _buildNumberText.Position = new Vector2(20.0f, Display.Height - 20.0f);
                
                if (Input.IsKeyDown(Keys.Escape))
                {
                    if (PauseMenu.Instance.Enabled)
                    {
                        DisablePause();
                    }
                    else
                    {
                        EnablePause();
                    }
                }
                
                if (Input.IsKeyDown(Keys.F8) && (PauseMenu.Instance == null || !PauseMenu.Instance.Enabled))
                {
                    Cursor.Show = !Cursor.Show;
                    Cursor.Lock = !Cursor.Show;
                }
                
                if (PhysicsManager.RayCast(Camera.Current.Position, Camera.Current.Forward, out var hit, 10.0f, 1))
                {
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
            try
            {
                Profiler.BeginProfile("DrawUI (.NET)");
                UIPanel.DrawAll();
                Profiler.EndProfile();
            }
            catch (Exception exception)
            {
                Exceptions.WriteException(exception);
            }
        }

        // shutdown
        protected override void Shutdown()
        {
        }

        /// <summary>
        /// Enables pause menu.
        /// </summary>
        public static void EnablePause()
        {
            Instance._crosshairBox.Enabled = false;

            Cursor.Show = true;
            Cursor.Lock = false;

            PauseMenu.Instance.Enable();
        }

        /// <summary>
        /// Disables pause menu.
        /// </summary>
        public static void DisablePause()
        {
            Instance._crosshairBox.Enabled = true;

            Cursor.Show = false;
            Cursor.Lock = true;

            PauseMenu.Instance.Disable();

            if (UIControl.FocusedControl != null)
                UIControl.SetFocusedControl(null);
        }
        
        /// <summary>
        /// Instance of main game class.
        /// </summary>
        public static GameMain Instance { get; private set; }
    }
}