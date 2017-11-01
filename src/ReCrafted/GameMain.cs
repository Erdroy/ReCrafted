// ReCrafted © 2016-2017 Always Too Late

using ReCrafted.API;
using ReCrafted.API.Common;
using ReCrafted.API.Core;
using ReCrafted.API.Graphics;
using ReCrafted.API.Mathematics;
using ReCrafted.API.UI;
using ReCrafted.Core.Locales;
using ReCrafted.Game.Core;

namespace ReCrafted.Game
{
    internal class GameMain : API.Core.Game
    {
        private Texture2D _crosshairTexture;
        private Font _testFont;
        private Entity _entity;

        protected override void Initialize()
        {
            // register unhandled exception handler
            Exceptions.RegisterUEHandler();

            _crosshairTexture = Texture2D.Create();
            _crosshairTexture.LoadFromFile("../assets/textures/crosshair.png");
            _crosshairTexture.Apply();

            _testFont = Font.Load("../assets/fonts/VeraMono.ttf", 20);

            Cursor.Show = false;
            Cursor.Lock = true;

            _entity = Entity.Create("New entity");
            _entity.AddScript<TestScript>();
            
            Locale.SetLocale("Polski");
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
           
            //var pos = new Vector2(20.0f, 20.0f);
            //UIInternal.DrawString(_testFont.NativePtr, Locale.Resolve("Test4"), ref pos);

            //UIPanel.DrawAll();
        }

        protected override void Shutdown()
        {
            Object.Destroy(_crosshairTexture);
        }
    }
}