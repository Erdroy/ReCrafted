// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

namespace ReCrafted.Core
{
    public interface IRendererComponent
    {
        void Init();
        void Tick();
        void Draw();
    }
}