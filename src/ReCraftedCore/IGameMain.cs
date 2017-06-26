// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

namespace ReCrafted
{
    public interface IGameMain
    {
        void Initialize();
        void Update();
        void Simulate();
        void Shutdown();
    }
}
