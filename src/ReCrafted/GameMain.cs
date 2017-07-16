// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

using ReCrafted.API;
using ReCrafted.API.Common;
using ReCrafted.API.Core;
using ReCrafted.API.Graphics;
using ReCrafted.API.UI;
using ReCrafted.Game.Core;

namespace ReCrafted.Game
{
    using Game = API.Core.Game;

    internal class GameMain : Game
    {
        private Camera _mainCamera;

        protected override void Initialize()
        {
            // register unhandled exception handler
            Exceptions.RegisterUEHandler();

            _mainCamera = Camera.Current;
        }

        protected override void Update()
        {
            if (Input.IsKeyDown(Keys.Space))
            {
                _mainCamera.Position = Vector3.Zero;
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
            UIInternal.Color = Color.DarkViolet;
            UIInternal.DrawBox(new RectangleF(Display.Width / 2.0f - 8.0f, Display.Height / 2.0f - 8.0f, 16.0f, 16.0f));

            UIPanel.DrawAll();
        }

        protected override void Shutdown()
        {

        }
    }
}