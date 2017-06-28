// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

using ReCrafted.API.Common;
using ReCrafted.API.Core;

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
            
        }

        protected override void Shutdown()
        {

        }
    }
}