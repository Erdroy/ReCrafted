// ReCrafted Editor (c) 2016-2018 Always Too Late

using System.Numerics;
using ImGuiNET;

namespace ReCrafted.Editor.Windows.Graph
{
    public class GraphWindowBase : DockableWindow
    {
        private DrawList _drawList;

        public override void Initialize()
        {
        }
        
        public override void OnRender()
        {
            _drawList = ImGui.GetWindowDrawList();
            
            ImGui.PushStyleColor(ColorTarget.WindowBg, new Vector4(0.0f, 0.0f, 0.0f, 1.0f));
            DrawNodesSurface();
            ImGui.PopStyleColor();
        }

        public override void Dispose()
        {
        }

        private void DrawNodesSurface()
        {

            var basePos = ImGui.GetWindowPosition();

            DrawNode(basePos + new Vector2(35.0f, 35.0f), new Vector2(140.0f, 80.0f));
            DrawNode(basePos + new Vector2(35.0f, 150.0f), new Vector2(140.0f, 80.0f));
        }

        private Vector2 DrawNode(Vector2 position, Vector2 size, string name = "<unknown-node>")
        {
            if (ImGui.InvisibleButton("##nodeHandle"))
            {
                return Vector2.Zero;
            }

            // Get text size
            var textSize = ImGui.GetTextSize(name);

            // Draw node background
            _drawList.AddRectFilled(position, position + size, 0xFF101010, 3.0f);

            // Draw node header
            _drawList.AddRectFilled(position, position + new Vector2(size.X, textSize.Y), 0xFF0069B5, 3.0f, 0x1 | 0x2);
            _drawList.AddText(position + new Vector2(size.X * 0.5f - textSize.X * 0.5f, 0.0f), name, 0xFFFFFFFF);

            return position;
        }

        public override string WindowName => "Node Graph";
    }
}
