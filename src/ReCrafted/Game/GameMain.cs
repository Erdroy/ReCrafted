// ReCrafted © 2016-2017 Always Too Late

using System;
using ReCrafted.API;
using ReCrafted.API.Common;
using ReCrafted.API.Core;
using ReCrafted.API.Graphics;
using ReCrafted.API.Mathematics;
using ReCrafted.API.UI;
using ReCrafted.Core;
using ReCrafted.Game.Core;
using Object = ReCrafted.API.Object;

namespace ReCrafted.Game
{
    internal class GameMain : API.Core.Game
    {
        private Texture2D _crosshairTexture;

        protected override void Initialize()
        {
            try
            {
                Instance = this;

                // register unhandled exception handler
                Exceptions.RegisterUEHandler();

                // load resources
                _crosshairTexture = Texture2D.Create(Assets.ResolveAssetFilePath(AssetType.Texture, "crosshair.png"));
                UIControl.DefaultFont = Font.Load(Assets.ResolveAssetFilePath(AssetType.Font, "VeraMono.ttf"), 12);

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

                TargetFps = 120;

                // pause Menu
                var mainEntity = Entity.Create("MainEntity");
                //mainEntity.AddScript<UITests>();
                mainEntity.AddScript<PauseMenu>();
                mainEntity.AddScript<SuperConsole>();
            }
            catch (Exception exception)
            {
                Exceptions.WriteException(exception);
            }
        }

        protected override void Update()
        {
            try
            {
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

                if (Input.IsKeyDown(Keys.F8) && !PauseMenu.Instance.Enabled)
                {
                    Cursor.Show = !Cursor.Show;
                    Cursor.Lock = !Cursor.Show;
                }

                DebugDraw.Color = new Color(0, 105, 0, 64);
                DebugDraw.DrawCube(Vector3.Zero, Vector3.One);
                DebugDraw.Color = new Color(0, 105, 0, 255);
                DebugDraw.DrawWireCube(Vector3.Zero, Vector3.One);

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

        protected override void Simulate()
        {
        }

        protected override void Draw()
        {
        }

        protected override void DrawUI()
        {
            try
            {
                Profiler.BeginProfile("DrawUI (.NET)");

                if (!PauseMenu.Instance.Enabled)
                {
                    UIInternal.Color = Color.White;
                    var rect = new RectangleF(Display.Width / 2.0f - 8.0f, Display.Height / 2.0f - 8.0f, 16.0f, 16.0f);
                    var uvs = new RectangleF(0.0f, 0.0f, 1.0f, 1.0f);
                    UIInternal.DrawTexture2D(_crosshairTexture.NativePtr, ref rect, ref uvs);
                }

                var pos = new Vector2(20.0f, Display.Height - 20.0f);
                UIInternal.Color = Color.White;
                UIInternal.DrawString(UIControl.DefaultFont.NativePtr, "ReCrafted " + GameInfo.Current.BuildName + " build " + GameInfo.Current.BuildNumber, ref pos);

                UIPanel.DrawAll();

                Profiler.BeginProfile("Sprite.DrawAll");
                Sprite.DrawAll();
                Profiler.EndProfile();

                Profiler.EndProfile();
            }
            catch (Exception exception)
            {
                Exceptions.WriteException(exception);
            }
        }

        protected override void Shutdown()
        {
            Object.Destroy(_crosshairTexture);
        }

        /// <summary>
        /// Enables pause menu.
        /// </summary>
        public static void EnablePause()
        {
            Cursor.Show = true;
            Cursor.Lock = false;

            PauseMenu.Instance.Enable();
        }

        /// <summary>
        /// Disables pause menu.
        /// </summary>
        public static void DisablePause()
        {
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