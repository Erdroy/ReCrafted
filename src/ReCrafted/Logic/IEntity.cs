// ReCrafted © 2016 Damian 'Erdroy' Korczowski

using System;

namespace ReCrafted.Logic
{
    public interface IEntity : IDisposable
    {
        void Init();
        void Tick();
        void Simulate();
        void Draw();

        void Destroy();
    }
}