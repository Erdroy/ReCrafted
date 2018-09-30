// ReCrafted Editor (c) 2016-2018 Always Too Late

using System.Drawing;
using ImGuiNET;

namespace ReCrafted.Editor.Windows
{
    public interface IWindow
    {
        void Initialize();
        void Update();
        void Render();
        void Dispose();

        int WindowId { get; }
        Rectangle WindowRect { get; }
        string WindowName { get; }
        WindowFlags WindowSettings { get; }
    }
}
