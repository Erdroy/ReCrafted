// ReCrafted Editor (c) 2016-2018 Always Too Late

using ImGuiNET;

namespace ReCrafted.Editor.Windows
{
    public class ContentWindow : WindowBase
    {
        public override void Initialize()
        {

        }

        public override void Update()
        {
        }

        public override void Render()
        {
            if (ImGui.BeginMenuBar())
            {
                if (ImGui.BeginMenu("Test1"))
                {
                    ImGui.EndMenu();
                }
                if (ImGui.BeginMenu("Test2"))
                {
                    ImGui.EndMenu();
                }
                if (ImGui.BeginMenu("Test3"))
                {
                    ImGui.EndMenu();
                }
                ImGui.EndMenuBar();
            }
            
        }

        public override void Dispose()
        {
        }

        public override string WindowName => "Content";
    }
}
