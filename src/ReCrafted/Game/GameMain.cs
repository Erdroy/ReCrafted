// ReCrafted © 2016-2017 Always Too Late

using ReCrafted.API;
using ReCrafted.API.Common;
using ReCrafted.API.Core;
using ReCrafted.API.Graphics;
using ReCrafted.API.Mathematics;
using ReCrafted.API.UI;
using ReCrafted.Core;
using ReCrafted.Core.Locales;
using ReCrafted.Game.Core;
using Object = ReCrafted.API.Object;

namespace ReCrafted.Game
{
    internal class GameMain : API.Core.Game
    {
        private Texture2D _crosshairTexture;

        protected override void Initialize()
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
            FreeCameraController.Current = camera.AddScript<FreeCameraController>();
            
            // load game info
            GameInfo.FromFile(Assets.ResolveAssetFilePath("gameinfo.json"));

            // apply locale
            Locale.SetLocale("Polski");

            // test ui script
            var tests = Entity.Create("UiTests");
            tests.AddScript<UITests>();

            // pause Menu
            var pauseMenu = Entity.Create("PauseMenu");
            pauseMenu.AddScript<PauseMenu>();
        }

        protected override void Update()
        {
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

                if (UIControl.FocusedControl != null)
                    UIControl.SetFocusedControl(null);
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
        }

        protected override void Simulate()
        {
        }

        protected override void Draw()
        {
        }

        protected override void DrawUI()
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

        protected override void Shutdown()
        {
            Object.Destroy(_crosshairTexture);
        }

        public static void EnablePause()
        {
            Cursor.Show = true;
            Cursor.Lock = false;

            PauseMenu.Instance.Enable();
        }

        public static void DisablePause()
        {
            Cursor.Show = false;
            Cursor.Lock = true;

            PauseMenu.Instance.Disable();

            if (UIControl.FocusedControl != null)
                UIControl.SetFocusedControl(null);
        }
        
        public static GameMain Instance { get; private set; }
    }
}