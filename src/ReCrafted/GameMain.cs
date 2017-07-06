// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

using ReCrafted.API.Common;
using ReCrafted.API.Core;
using ReCrafted.API.UI;

namespace ReCrafted
{
    internal class GameMain : Game
    {
        protected override void Initialize()
        {
            Logger.Write("Hello, World from C#!", LogLevel.Debug);
        }

        protected override void Update()
        {
            if (Input.IsKeyDown(Keys.F1))
            {
                Quit();
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
        }

        protected override void Shutdown()
        {

        }
    }
}