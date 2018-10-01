// ReCrafted Editor (c) 2016-2018 Always Too Late

using ReCrafted.Editor.Windows.Docking;

namespace ReCrafted.Editor.Windows.Layout
{
    public class LayoutDescription
    {
        public class Window
        {
            public int Id { get; set; }
            public string Type { get; set; }
            public bool Floating { get; set; }
            public int X { get; set; }
            public int Y { get; set; }
            public int Width { get; set; }
            public int Height { get; set; }
        }

        public class LayoutInfo
        {
            public DockType DockType { get; set; }

            public int WindowAId { get; set; }
            public int WindowBId { get; set; }

            public LayoutInfo ChildrenA { get; set; }
            public LayoutInfo ChildrenB { get; set; }

            public float DockSize { get; set; }
        }

        public Window[] Windows;
        public LayoutInfo Layout;
    }
}
