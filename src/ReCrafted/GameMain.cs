// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

using ReCrafted.API.Common;
using ReCrafted.API.Graphics;
using ReCrafted.API.Mathematics;
using ReCrafted.API.UI;
using ReCrafted.Game.Core;

namespace ReCrafted.Game
{
    using Game = API.Core.Game;

    internal class GameMain : Game
    {
        private Texture2D _crosshairTexture;

        protected override void Initialize()
        {
            // register unhandled exception handler
            Exceptions.RegisterUEHandler();

            _crosshairTexture = Texture2D.Create();
            _crosshairTexture.LoadFromFile("../Assets/textures/crosshair.png");
            _crosshairTexture.Apply();
        }

        protected override void Update()
        {

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

            UIPanel.DrawAll();
        }

        protected override void Shutdown()
        {

        }
    }
}