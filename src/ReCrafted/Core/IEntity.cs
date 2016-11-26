// ReCrafted © 2016 Damian 'Erdroy' Korczowski

using System;

namespace ReCrafted.Core
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