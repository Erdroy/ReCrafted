// ReCrafted Editor (c) 2016-2018 Always Too Late

using System.Numerics;
using ImGuiNET;

namespace ReCrafted.Editor.Panels
{
    public class ContentPanel : PanelBase
    {
        public override void Initialize()
        {

        }

        public override void Update()
        {
        }

        public override void Render()
        {
            ImGui.BeginWindow("Content");
            
            ImGui.EndWindow();
        }

        public override void Dispose()
        {
        }
    }
}
