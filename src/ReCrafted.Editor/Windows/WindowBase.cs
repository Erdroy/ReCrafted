// ReCrafted Editor (c) 2016-2018 Always Too Late

using ImGuiNET;

namespace ReCrafted.Editor.Windows
{
    public abstract class WindowBase
    {
        public abstract void Initialize();
        public abstract void Update();
        public abstract void Render();
        public abstract void Dispose();

        public abstract string WindowName { get; }
        public virtual WindowFlags WindowSettings => WindowFlags.NoCollapse;
    }
}
