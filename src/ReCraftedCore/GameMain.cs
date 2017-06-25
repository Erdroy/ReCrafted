// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

using ReCrafted.API.Core;

namespace ReCraftedCore
{
    internal class GameMain : IGameMain
    {
        public void Initialize()
        {
            Logger.Write("Hello, World! From C#!", LogLevel.Error);
        }

        public void Update()
        {

        }

        public void Simulate()
        {

        }

        public void Shutdown()
        {

        }
    }
}