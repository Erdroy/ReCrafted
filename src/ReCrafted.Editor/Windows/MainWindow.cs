// ReCrafted Editor (c) 2016-2018 Always Too Late

using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Numerics;
using ImGuiNET;
using ReCrafted.Editor.Content;
using ReCrafted.Editor.Content.Assets;
using ReCrafted.Editor.Core;
using ReCrafted.Editor.Windows.Docking;

namespace ReCrafted.Editor.Windows
{
    public class MainWindow : WindowBase
    {
        public MainWindow()
        {
            Instance = this;
        }

        public override void Initialize()
        {
            // Setup style
            var style = ImGui.GetStyle();

            style.WindowRounding = 3.0f;
            style.ScrollbarRounding = 2.0f;

            style.SetColor(ColorTarget.TitleBg, new Vector4(0.25f, 0.25f, 0.25f, 1.0f));
            style.SetColor(ColorTarget.TitleBgCollapsed, new Vector4(0.28f, 0.28f, 0.28f, 1.0f));
            style.SetColor(ColorTarget.TitleBgActive, new Vector4(0.28f, 0.28f, 0.28f, 1.0f));
            style.SetColor(ColorTarget.WindowBg, new Vector4(0.15f, 0.15f, 0.15f, 1.0f));
            style.SetColor(ColorTarget.Border, new Vector4(0.10f, 0.10f, 0.10f, 1.0f));

            style.SetColor(ColorTarget.PopupBg, new Vector4(0.10f, 0.10f, 0.10f, 1.0f));

            style.SetColor(ColorTarget.MenuBarBg, new Vector4(0.20f, 0.20f, 0.20f, 1.0f));
            style.SetColor(ColorTarget.Header, new Vector4(0.28f, 0.28f, 0.28f, 1.0f));
            style.SetColor(ColorTarget.HeaderActive, new Vector4(0.28f, 0.28f, 0.28f, 1.0f));
            style.SetColor(ColorTarget.HeaderHovered, new Vector4(0.25f, 0.25f, 0.25f, 1.0f));

            style.SetColor(ColorTarget.ScrollbarBg, new Vector4(0.18f, 0.18f, 0.18f, 1.0f));
            style.SetColor(ColorTarget.ScrollbarGrab, new Vector4(0.25f, 0.25f, 0.25f, 1.0f));
            style.SetColor(ColorTarget.ScrollbarGrabActive, new Vector4(0.28f, 0.28f, 0.28f, 1.0f));
            style.SetColor(ColorTarget.ScrollbarGrabHovered, new Vector4(0.28f, 0.28f, 0.28f, 1.0f));

            // Setup docking
            DockPane = new DockPane();
            DockPane.Resize(new Rectangle(0, 0,
                EditorApplication.Current.Window.Width,
                EditorApplication.Current.Window.Height));
            
            var s1 = DockPane.Dock(new DockPanelBase(), DockType.Fill, DockDirection.Right);

            var s2 = s1.Dock(new DockPanelBase(), DockType.Horizontal, DockDirection.Down);
            
            var s3 = s2.Dock(new DockPanelBase(), DockType.Vertical, DockDirection.Right);
            
            s3.Dock(new DockPanelBase(), DockType.Vertical, DockDirection.Left);

            // Setup events
            EditorApplication.Current.Window.Resized += MainWindowResized;
        }

        private void MainWindowResized()
        {
            // Resize dock pane
            DockPane.Resize(new Rectangle(0, 0,
                EditorApplication.Current.Window.Width,
                EditorApplication.Current.Window.Height));
        }

        public override void Update()
        {
            foreach (var child in Children)
            {
                child.Update();
            }
        }
        
        public override void Render()
        {
            ImGui.PushStyleVar(StyleVar.PopupBorderSize, 0.0f);

            var opened = true;
            ImGui.BeginWindow("Main", ref opened,
                new Vector2(EditorApplication.Current.Window.Width, EditorApplication.Current.Window.Height),
                0.0f,
                WindowFlags.NoResize | WindowFlags.NoInputs | WindowFlags.NoMove | WindowFlags.NoTitleBar |
                WindowFlags.NoScrollbar);

            DockPane.DebugDraw();
            
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
                    if (ImGui.MenuItem("Test - Create json asset"))
                    {
                        var asset = ContentManager.CreateAsset<TestAsset>();

                        using (var fs = new FileStream("../content/jsonasset.rcasset", FileMode.Create,
                            FileAccess.Write))
                        {
                            asset.Serialize(fs);
                        }

                        asset.Unload();
                    }

                    if (ImGui.MenuItem("Test - Load asset"))
                    {
                        var asset = ContentManager.Load<TextureAsset>("../content/TestTexture.rcasset");
                    }

                    ImGui.EndMenu();
                }

                if (ImGui.BeginMenu("Help"))
                {
                    ImGui.EndMenu();
                }

                ImGui.EndMainMenuBar();
            }

            ImGui.PushStyleVar(StyleVar.WindowBorderSize, 1.0f);

            foreach (var child in Children)
            {
                if (ImGui.BeginWindow(child.WindowName, child.WindowSettings))
                {
                    child.Render();
                    ImGui.EndWindow();
                }
            }

            ImGui.PopStyleVar();

            ImGui.EndWindow();
        }

        public override void Dispose()
        {
            foreach (var child in Children)
                child.Dispose();
        }

        public DockPane DockPane { get; private set; }

        public override string WindowName => "MainWindow";

        public List<WindowBase> Children { get; } = new List<WindowBase>();

        public static MainWindow Instance { get; private set; }
    }
}
