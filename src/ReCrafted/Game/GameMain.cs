// ReCrafted © 2016-2017 Always Too Late

using System.Diagnostics;
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

            // Load resources.
            _crosshairTexture = Texture2D.Create(Assets.ResolveAssetFilePath(AssetType.Texture, "crosshair.png"));
            UIControl.DefaultFont = Font.Load(Assets.ResolveAssetFilePath(AssetType.Font, "VeraMono.ttf"), 12);

            Cursor.Show = false;
            Cursor.Lock = true;

            // Create camera.
            var camera = Entity.Create("MainCamera");
            camera.Position = new Vector3(35.0f, 925.0f, 62.0f);
            FreeCameraController.Current = camera.AddScript<FreeCameraController>();
            
            // Load game info
            GameInfo.FromFile(Assets.ResolveAssetFilePath("gameinfo.json"));

            // Apply locale
            Locale.SetLocale("Polski");

            // Test ui script
            var tests = Entity.Create("UiTests");
            tests.AddScript<UITests>();
        }

        protected override void Update()
        {
            if (Input.IsKeyDown(Keys.Escape))
            {
                if (Cursor.Show)
                {
                    Cursor.Show = false;
                    Cursor.Lock = true;
                }
                else
                {
                    Cursor.Show = true;
                    Cursor.Lock = false;
                }

                if (UIControl.FocusedControl != null)
                    UIControl.SetFocusedControl(null);
            }

            /*
            if (Input.IsKeyDown(Keys.Mouse0) || Input.IsKeyDown(Keys.Mouse1))
            {
                Cursor.Show = false;
                Cursor.Lock = true;
            }
            */
            
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

        // total miliseconds needs to calculate current ui
        internal static double LastMilisecondsForUiPanel;
        internal static double TotalMilisecondsForUiPanel;
        internal static long TotalFramesOfUiPanel;

        protected override void DrawUI()
        {
            var sw = Stopwatch.StartNew();
            UIPanel.DrawAll();
            sw.Stop();
            LastMilisecondsForUiPanel = sw.ElapsedMilliseconds;
            TotalMilisecondsForUiPanel += sw.ElapsedMilliseconds;
            TotalFramesOfUiPanel++;
            if (TotalFramesOfUiPanel > 1000)// clear every 1k frames
            {
                TotalFramesOfUiPanel = 0;
                TotalMilisecondsForUiPanel = 0;
            }

            UIInternal.Color = Color.White;
            var rect = new RectangleF(Display.Width / 2.0f - 8.0f, Display.Height / 2.0f - 8.0f, 16.0f, 16.0f);
            var uvs = new RectangleF(0.0f, 0.0f, 1.0f, 1.0f);
            UIInternal.DrawTexture2D(_crosshairTexture.NativePtr, ref rect, ref uvs);

            var pos = new Vector2(20.0f, Display.Height - 20.0f);
            UIInternal.DrawString(UIControl.DefaultFont.NativePtr, "ReCrafted " + GameInfo.Current.BuildName + " build " + GameInfo.Current.BuildNumber, ref pos);
        }

        protected override void Shutdown()
        {
            Object.Destroy(_crosshairTexture);
        }
        
        public static GameMain Instance { get; private set; }
    }
}