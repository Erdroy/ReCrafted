// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

using ReCrafted.API;
using ReCrafted.API.Common;
using ReCrafted.API.Core;
using ReCrafted.API.Graphics;
using ReCrafted.API.Mathematics;
using ReCrafted.API.UI;
using ReCrafted.Game.Core;

namespace ReCrafted.Game
{
    using Game = API.Core.Game;

    internal class GameMain : Game
    {
        private Camera _mainCamera;

        private Camera _camera;

        protected override void Initialize()
        {
            // register unhandled exception handler
            Exceptions.RegisterUEHandler();

            _mainCamera = Camera.Current;

            _camera = Object.Create<Camera>();
        }

        protected override void Update()
        {
            if (Input.IsKeyDown(Keys.Space))
            {
                _mainCamera.Position = Vector3.Zero;
            }

            if (Input.IsKeyDown(Keys.F))
            {
                _camera.SetAsCurrent();
            }

            if (Input.IsKeyUp(Keys.F))
            {
                _mainCamera.SetAsCurrent();
            }

            _camera.Forward = -_mainCamera.Forward;
            _camera.Position = _mainCamera.Position;
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