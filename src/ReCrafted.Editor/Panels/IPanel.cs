// ReCrafted Editor (c) 2016-2018 Always Too Late

using System;

namespace ReCrafted.Editor.Panels
{
    public interface IPanel : IDisposable
    {
        void Initialize();
        void Update();
        void Render();
    }
}
