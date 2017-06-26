// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

using ReCrafted.API.Common;
using ReCrafted.API.Core;

namespace ReCraftedCore
{
    internal class GameMain : IGameMain
    {
        public void Initialize()
        {
            Logger.Write("Hello, World! From C#!", LogLevel.Debug);
        }

        public void Update()
        {
            if (Input.IsKeyDown(Keys.Mouse0))
            {
                Logger.Write("Mouse0 down");
            }
        }

        public void Simulate()
        {

        }

        public void Shutdown()
        {

        }
    }
}