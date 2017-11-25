// ReCrafted © 2016-2017 Always Too Late

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

namespace ReCrafted.Game
{
    internal class GameMain : API.Core.Game
    {
        private Texture2D _crosshairTexture;
        private Font _testFont;

        protected override void Initialize()
        {
            Instance = this;

            // register unhandled exception handler
            Exceptions.RegisterUEHandler();

            _crosshairTexture = Texture2D.CreateAndLoad("../assets/textures/crosshair.png");

            _testFont = Font.Load("../assets/fonts/VeraMono.ttf", 12);

            Cursor.Show = false;
            Cursor.Lock = true;

            var camera = Entity.Create("MainCamera");
            camera.Position = new Vector3(0.0f, 1224.0f, 0.0f);
            FreeCameraController.Current = camera.AddScript<FreeCameraController>();
            
            // Load game info
            GameInfo.FromFile("../assets/gameinfo.json");

            Locale.SetLocale("Polski");
            
            //var panel = UIPanel.Create(new RectangleF(10.0f, 10.0f, 200.0f, 450.0f), UILayoutType.Grid);
            //panel.Enabled = true;
            //panel.Visible = true;
            //panel.Layout.Add(new UIBox());
        }

        protected override void Update()
        {
            if (Input.IsKeyDown(Keys.Escape))
            {
                Cursor.Show = true;
                Cursor.Lock = false;
            }

            if (Input.IsKeyDown(Keys.Mouse0) || Input.IsKeyDown(Keys.Mouse1))
            {
                Cursor.Show = false;
                Cursor.Lock = true;
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
            var rect = new RectangleF(Display.Width / 2.0f - 8.0f, Display.Height / 2.0f - 8.0f, 16.0f, 16.0f);
            var uvs = new RectangleF(0.0f, 0.0f, 1.0f, 1.0f);
            UIInternal.DrawTexture2D(_crosshairTexture.NativePtr, ref rect, ref uvs);

            var pos = new Vector2(20.0f, Display.Height - 20.0f);
            UIInternal.DrawString(_testFont.NativePtr, "ReCrafted " + GameInfo.Current.BuildName + " build " + GameInfo.Current.BuildNumber, ref pos);

            UIPanel.DrawAll();
        }

        protected override void Shutdown()
        {
            Object.Destroy(_crosshairTexture);
        }
        
        public static GameMain Instance { get; private set; }
    }
}