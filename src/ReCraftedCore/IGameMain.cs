// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

namespace ReCraftedCore
{
    public interface IGameMain
    {
        void Initialize();
        void Update();
        void Simulate();
        void Shutdown();
    }
}
