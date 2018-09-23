// ReCrafted Editor (c) 2016-2018 Always Too Late

namespace ReCrafted.Editor.Panels
{
    public abstract class PanelBase : IPanel
    {
        public abstract void Initialize();
        public abstract void Update();
        public abstract void Render();
        public abstract void Dispose();

        public abstract string WindowName { get; }
    }
}
