// ReCrafted (c) 2016-2018 Always Too Late

using System;
using ReCrafted.API;
using ReCrafted.API.Common;
using ReCrafted.API.Core;
using ReCrafted.API.Graphics;
using ReCrafted.API.Mathematics;
using ReCrafted.API.UI;
using ReCrafted.API.UI.Controls;
using ReCrafted.Common;
using ReCrafted.Game.Core;
using ReCrafted.Game.Interface;
using ReCrafted.Game.Super;

namespace ReCrafted.Game
{
    internal class GameMain : Application
    {
        // cross hair control
        private UIBox _crosshairBox;

        // build number control
        private UIText _buildNumberText;

        private UITests2 _tests2;

        private WebUIView _uiView;

        // initialize
        protected override void Initialize()
        {
            try
            {
                Instance = this;

                // register unhandled exception handler
                Exceptions.RegisterUEHandler();

                // load resources

                // initialize default ui panel
                UIControl.Init();

                Cursor.Show = false;
                Cursor.Lock = true;

                // create camera
                var camera = Entity.Create("MainCamera");
                camera.Position = new Vector3(35.0f, 925.0f, 62.0f);
                camera.AddScript<FreeCameraController>();

                // load game info
                GameInfo.FromFile(Assets.ResolveAssetFilePath("gameinfo.json"));

                // apply locale
                //Locale.SetLocale("Polski");

                // apply target fps
                TargetFps = 60;

                // create some default controls
                _crosshairBox =
                    UIControl.CreateControl(
                        new UIBox(Sprite.Create(Assets.ResolveAssetFilePath(AssetType.Interface, "crosshair.png"))));

                _buildNumberText = UIControl.CreateControl(new UIText());
                _buildNumberText.Text =
                    "ReCrafted " + GameInfo.Current.BuildName + " build " + GameInfo.Current.BuildNumber;

                // initialize default scripts
                // pause Menu
                var mainEntity = Entity.Create("MainEntity");
                // mainEntity.AddScript<UITests>();
                mainEntity.AddScript<SuperConsole>();
                mainEntity.AddScript<PauseMenu>();
                mainEntity.AddScript<Messenger>();

                // create example webui view
                _uiView = WebUI.Create();
                _uiView.Navigate("file:///ui/default.html");
                //_uiView.Navigate("file:///ui/menu/menu.html");

                _tests2 = new UITests2();
                _tests2.Init();
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
                _tests2.Update();

                _crosshairBox.Region =
                    new RectangleF(Display.Width / 2.0f - 8.0f, Display.Height / 2.0f - 8.0f, 16.0f, 16.0f);
                _buildNumberText.Position = new Vector2(20.0f, Display.Height - 20.0f);

                if (Input.IsKeyDown(Keys.Escape))
                {
                    if (SuperConsole.Instance.Enabled)
                        DisableConsole();
                    else
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
                }

                if (Input.IsKeyDown(Keys.OEM3))
                {
                    if (SuperConsole.Instance.Enabled)
                    {
                        DisableConsole();
                    }
                    else
                    {
                        EnableConsole();
                    }
                }

                if (Input.IsKeyDown(Keys.F8) && (PauseMenu.Instance == null || !PauseMenu.Instance.Enabled))
                {
                    Cursor.Show = !Cursor.Show;
                    Cursor.Lock = !Cursor.Show;
                }

                if (UIControl.FocusedControl == null)
                {
                    if (Input.IsKeyDown(Keys.G))
                        Messenger.ShowCenterMessage("Test!", "Lul", 4f, null);

                    if (Input.IsKeyDown(Keys.H))
                        Messenger.ShowCenterMessage("Test!", "Lul", 4f, button =>
                        {
                            Messenger.ShowCenterMessage(button.ToString() + "!", "ClickEvent!", 4f, null);
                        }, MessageType.Error, MessageButtons.OkNoCancel);
                }

                DebugDraw.Color = new Color(0, 105, 0, 64);
                DebugDraw.DrawBox(Vector3.Zero, Vector3.One);
                DebugDraw.Color = new Color(0, 105, 0, 255);
                DebugDraw.DrawWireBox(Vector3.Zero, Vector3.One);

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
        /// Enables console.
        /// </summary>
        public static void EnableConsole()
        {
            Cursor.Show = true;
            Cursor.Lock = false;

            SuperConsole.Instance.Enable();
        }

        /// <summary>
        /// Disables console.
        /// </summary>
        public static void DisableConsole()
        {
            Cursor.Show = false;
            Cursor.Lock = true;

            SuperConsole.Instance.Disable();

            if (UIControl.FocusedControl != null)
                UIControl.SetFocusedControl(null);
        }

        /// <summary>
        /// Instance of main game class.
        /// </summary>
        public static GameMain Instance { get; private set; }
    }
}