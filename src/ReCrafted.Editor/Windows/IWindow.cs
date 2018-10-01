// ReCrafted Editor (c) 2016-2018 Always Too Late

using System.Drawing;
using ImGuiNET;

namespace ReCrafted.Editor.Windows
{
    public interface IWindow
    {
        void Initialize();
        void Render();
        void Dispose();

        void Open();
        void Hide();
        void Close();
        
        bool WindowClosed { get; }
        bool WindowVisible { get; }
        int WindowId { get; }
        Rectangle WindowRect { get; }
        string WindowName { get; }
        WindowFlags WindowSettings { get; }
    }
}
