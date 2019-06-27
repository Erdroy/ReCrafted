// ReCrafted Editor (c) 2016-2018 Always Too Late

namespace ReCrafted.Editor.Windows.Docking
{
    public interface IDockHandler
    {
        DockPanelBase Dock(DockPanelBase other, DockType dockType, DockDirection dockDirection, float sizeMul = 0.5f);
    }
}
