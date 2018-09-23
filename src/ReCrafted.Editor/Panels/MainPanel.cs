// ReCrafted Editor (c) 2016-2018 Always Too Late

using System.Windows.Forms;
using System.Collections.Generic;
using System.Numerics;
using ImGuiNET;
using ReCrafted.Editor.Content.Importers;
using ReCrafted.Editor.Core;

namespace ReCrafted.Editor.Panels
{
    public class MainPanel : PanelBase
    {
        public MainPanel()
        {
            Instance = this;
        }

        public override void Initialize()
        {
        }

        public override void Update()
        {
            foreach (var child in ChildrenPanels)
            {
                ImGui.BeginWindow(child.WindowName, WindowFlags.MenuBar);
                child.Update();
                ImGui.EndWindow();
            }
        }

        public override void Render()
        {
            var opened = true;
            ImGui.SetNextWindowPos(new Vector2(0.0f, 0.0f), Condition.Always, Vector2.Zero);
            ImGui.BeginWindow("Main", ref opened, new Vector2(EditorApplication.Current.Window.Width, EditorApplication.Current.Window.Height), 
                0.0f, WindowFlags.NoResize | WindowFlags.NoInputs | WindowFlags.NoMove | WindowFlags.NoTitleBar | WindowFlags.NoScrollbar);
            
            if (ImGui.BeginMainMenuBar())
            {
                if (ImGui.BeginMenu("File"))
                {
                    if (ImGui.MenuItem("Exit"))
                    {
                        ApplicationBase.Current.Shutdown();
                    }
                    ImGui.EndMenu();
                }

                if (ImGui.BeginMenu("Edit"))
                {
                    ImGui.EndMenu();
                }

                if (ImGui.BeginMenu("Tools"))
                {
                    if (ImGui.MenuItem("Import asset"))
                    {
                        using (OpenFileDialog openFileDialog1 = new OpenFileDialog())
                        {
                            openFileDialog1.InitialDirectory = "c:\\";
                            openFileDialog1.Filter = "png files (*.png)|*.png|All files (*.*)|*.*";
                            openFileDialog1.FilterIndex = 2;
                            openFileDialog1.RestoreDirectory = true;

                            if (openFileDialog1.ShowDialog() == DialogResult.OK)
                            {
                                var ci = new TextureImporter();
                                ci.ImportAsset(openFileDialog1.FileName, "Texture.rcasset");
                            }
                        }
                    }

                    ImGui.EndMenu();
                }

                if (ImGui.BeginMenu("Help"))
                {
                    ImGui.EndMenu();
                }

                ImGui.EndMainMenuBar();
            }
            
            foreach (var child in ChildrenPanels)
                child.Render();

            ImGui.EndWindow();
        }

        public override void Dispose()
        {
            foreach (var child in ChildrenPanels)
                child.Dispose();
        }

        public override string WindowName => "MainWindow";

        public List<PanelBase> ChildrenPanels { get; } = new List<PanelBase>();

        public static MainPanel Instance { get; private set; }
    }
}
