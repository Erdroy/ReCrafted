// ReCrafted Editor (c) 2016-2018 Always Too Late

using System.Numerics;
using ImGuiNET;
using ReCrafted.Editor.Common;

namespace ReCrafted.Editor.Windows.Graph
{
    public class GraphWindowBase : DockableWindow
    {
        private ImDrawListPtr _drawList;

        public override void Initialize()
        {
        }
        
        public override void OnRender()
        {
            _drawList = ImGui.GetWindowDrawList();
            
            ImGui.PushStyleColor(ImGuiCol.WindowBg, new Vector4(0.0f, 0.0f, 0.0f, 1.0f));
            DrawNodesSurface();
            ImGui.PopStyleColor();
        }

        public override void Dispose()
        {
        }

        private Vector2 _nodePosition1 = new Vector2(35f, 150f);
        private Vector2 _nodePosition2 = new Vector2(35f, 250f);

        private void DrawNodesSurface()
        {
            var basePos = ImGui.GetWindowPos();

            _nodePosition1 = DrawNode(basePos + _nodePosition1, new Vector2(140.0f, 80.0f), "<unknown-node1>") -
                             basePos;
            _nodePosition2 = DrawNode(basePos + _nodePosition2, new Vector2(140.0f, 80.0f), "<unknown-node2>") -
                             basePos;

           // _drawList.AddBezierCurve(_nodePosition1 + basePos, _nodePosition1 + new Vector2(20, 0) + basePos,  _nodePosition2 + new Vector2(-20, 0) + basePos, _nodePosition2 + basePos, 0xFF0069B5, 3.0f, 0x1 | 0x2);
        }

        private Vector2 DrawNode(Vector2 position, Vector2 size, string name = "<unknown-node>")
        {
            ImGui.SetCursorScreenPos(position);
            ImGui.InvisibleButton(name, size);
            if (ImGui.IsItemActive() && ImGui.IsMouseDragging(0))
            {
                position += ImGui.GetMouseDragDelta(0);
                ImGui.ResetMouseDragDelta(0);
            }

            // Get text size
            var textSize = ImGui.CalcTextSize(name);

            // Draw node background
            _drawList.AddRectFilled(position, position + size, 0xFF101010, 3.0f);

            // Draw node header
            _drawList.AddRectFilled(position, position + new Vector2(size.X, textSize.Y), 0xFF0069B5, 3.0f,  0x1 | 0x2);
            _drawList.AddText(position + new Vector2(size.X * 0.5f - textSize.X * 0.5f, 0.0f), 0xFFFFFFFF, name);

            return position;
        }

        public override string WindowName => "Node Graph";
    }
}
