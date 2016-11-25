// ReCrafted © 2016 Damian 'Erdroy' Korczowski

namespace ReCrafted.Core
{
    public interface IRendererComponent
    {
        void Init();
        void Tick();
        void Draw();
    }
}