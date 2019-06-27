// ReCrafted Editor (c) 2016-2018 Always Too Late

using System.Diagnostics;
using System.Drawing;
using System.Numerics;
using ImGuiNET;

namespace ReCrafted.Editor.Windows.Docking
{
    public class DockingHelpers
    {
        public static DockPanelBase TryFindDockTarget(ref DockType type, ref DockDirection direction)
        {
            var mousePos = ImGui.GetMousePos();

            // Try to find drop surface
            var panel = MainWindow.Instance.DockPane.FindIntersecting(mousePos);

            // Check if panel exists or it's parent is null
            // When panels parent is null, it is the ROOT!
            if (panel?.Parent == null)
            {
                return null;
            }

            Debug.Assert(!(panel is DockSplitter));

            var drawList = ImGui.GetForegroundDrawList();
            var rect = panel.Rect;

            var innerWidth = (int)(rect.Width * 0.30f);
            var innerHeight = (int)(rect.Height * 0.30f);

            var dockType = DockType.None;
            var dockDir = DockDirection.None;

            // Check top // TODO: Docking on top
            /*var top = new Rectangle(rect.Left, rect.Top, rect.Width, 20);
            if (dockType == DockType.None && top.Contains((int)mousePos.X, (int)mousePos.Y))
            {
                drawList.AddRectFilled(
                    new Vector2(top.Left, top.Top),
                    new Vector2(top.Right, top.Bottom),
                    0x4FFF6900, 0.0f);
            }*/

            // Check bottom
            var bottom = new Rectangle(rect.Left + innerWidth, rect.Bottom - innerHeight,
                rect.Width - innerWidth * 2, innerHeight);
            if (dockType == DockType.None && bottom.Contains((int)mousePos.X, (int)mousePos.Y))
            {
                drawList.AddRectFilled(
                    new Vector2(rect.Left, rect.Bottom - innerHeight),
                    new Vector2(rect.Right, rect.Bottom),
                    0x4FFF6900, 0.0f);
                dockType = DockType.Horizontal;
                dockDir = DockDirection.Down;
            }

            // Check left
            var left = new Rectangle(rect.Left, rect.Top, innerWidth, rect.Height);
            if (dockType == DockType.None && left.Contains((int)mousePos.X, (int)mousePos.Y))
            {
                drawList.AddRectFilled(
                    new Vector2(left.Left, left.Top),
                    new Vector2(left.Right, left.Bottom),
                    0x4FFF6900, 0.0f);
                dockType = DockType.Vertical;
                dockDir = DockDirection.Left;
            }

            // Check right
            var right = new Rectangle(rect.Right - innerWidth, rect.Top, innerWidth, rect.Height);
            if (dockType == DockType.None && right.Contains((int)mousePos.X, (int)mousePos.Y))
            {
                drawList.AddRectFilled(
                    new Vector2(right.Left, right.Top),
                    new Vector2(right.Right, right.Bottom),
                    0x4FFF6900, 0.0f);
                dockType = DockType.Vertical;
                dockDir = DockDirection.Right;
            }

            // Set docking values
            if (dockType != DockType.None)
            {
                type = dockType;
                direction = dockDir;
                return panel;
            }

            return null;
        }
    }
}
