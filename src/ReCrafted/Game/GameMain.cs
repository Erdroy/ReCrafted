// ReCrafted © 2016-2017 Always Too Late

using System;
using ReCrafted.API;
using ReCrafted.API.Common;
using ReCrafted.API.Core;
using ReCrafted.API.Graphics;
using ReCrafted.API.Mathematics;
using ReCrafted.API.UI;
using ReCrafted.API.UI.Controls;
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
            // var tests = Entity.Create("Tests");
            // tests.AddScript<UITests>();
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

        private const bool EnableUiMs = false;
        private const int UiProcessDebugFrames = 30;

        private static double _uiProcessDebugMs;
        private static double _uiProcessDebugTotal;
        private static double _uiProcessDebugLastMs;
        private static float _uiProcessDebugTime;

        protected override void DrawUI()
        {
            if (EnableUiMs)
            {
                var start = DateTime.Now;
                UIPanel.DrawAll();
                var last = (DateTime.Now - start).TotalMilliseconds;
                _uiProcessDebugMs += last;
                if (_uiProcessDebugTime < 1)
                    _uiProcessDebugTime += (float)Time.DeltaTime;
                else
                {
                    _uiProcessDebugLastMs = last;
                    _uiProcessDebugTime = 0f;
                    _uiProcessDebugTotal = _uiProcessDebugMs / 10;
                    _uiProcessDebugMs = 0;
                }
            }
            else
            {
                UIPanel.DrawAll();
            }

            UIInternal.Color = Color.White;
            var rect = new RectangleF(Display.Width / 2.0f - 8.0f, Display.Height / 2.0f - 8.0f, 16.0f, 16.0f);
            var uvs = new RectangleF(0.0f, 0.0f, 1.0f, 1.0f);
            UIInternal.DrawTexture2D(_crosshairTexture.NativePtr, ref rect, ref uvs);

            var pos = new Vector2(20.0f, Display.Height - 20.0f);
            UIInternal.DrawString(UIControl.DefaultFont.NativePtr,
                "ReCrafted " + GameInfo.Current.BuildName + " build " + GameInfo.Current.BuildNumber, ref pos);

            if (EnableUiMs)
            {
                pos = new Vector2(20.0f, Display.Height - 40.0f);
                UIInternal.DrawString(UIControl.DefaultFont.NativePtr,
                    "Ui process took -> " + _uiProcessDebugTotal + " ms / " + UiProcessDebugFrames + " frames (last " + _uiProcessDebugLastMs + " ms )", ref pos);
            }
        }

        protected override void Shutdown()
        {
            Object.Destroy(_crosshairTexture);
        }
        
        public static GameMain Instance { get; private set; }
    }
}