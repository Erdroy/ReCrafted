// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

namespace ReCrafted.Core
{
    public interface IGameComponent
    {
        void Init();
        void Tick();
        void Draw();
    }
}