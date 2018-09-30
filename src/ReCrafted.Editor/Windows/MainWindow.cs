// ReCrafted Editor (c) 2016-2018 Always Too Late

using System;
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
    public class MainWindow : IWindow, IDisposable
    {
        private static int _lastWindowId = 1;

        public MainWindow()
        {
            Instance = this;
        }

        public void Initialize()
        {
            // Setup style
            var style = ImGui.GetStyle();

            style.ScrollbarRounding = 2.0f;
            style.WindowPadding = new Vector2(4.0f, 4.0f);

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
            DockPane.Resize(WindowRect);
            
            // Setup events
            EditorApplication.Current.SdlWindow.Resized += MainWindowResized;
        }

        private void MainWindowResized()
        {
            // Resize dock pane
            DockPane.Resize(WindowRect);
        }

        public void Update()
        {
            foreach (var child in Children)
            {
                child.Update();
            }
        }
        
        public void Render()
        {
            ImGui.PushStyleVar(StyleVar.PopupBorderSize, 0.0f);

            var opened = true;
            var windowRect = WindowRect;

            ImGui.BeginWindow("Main", ref opened, new Vector2(windowRect.Width, windowRect.Height), 0.0f, WindowSettings);
            
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
                    if (ImGui.MenuItem("Undo"))
                    {
                    }

                    if (ImGui.MenuItem("Redo"))
                    {
                    }

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
            
            // Render children windows
            foreach (var child in Children)
            {
                child.Render();
            }

            ImGui.PopStyleVar();

            ImGui.EndWindow();
        }

        public void Dispose()
        {
            foreach (var child in Children)
                child.Dispose();
        }

        public int WindowId => 0;

        public T AddChildren<T>() where T : DockableWindow, new()
        {
            var window = new T
            {
                WindowId = _lastWindowId++
            };

            window.Initialize();
            window.Focus();
            Children.Add(window);
            return window;
        }
        
        public Rectangle WindowRect => new Rectangle(0, 20,
            EditorApplication.Current.SdlWindow.Width,
            EditorApplication.Current.SdlWindow.Height - 20);

        public string WindowName => "MainWindow";
        public WindowFlags WindowSettings => WindowFlags.NoResize | WindowFlags.NoInputs | WindowFlags.NoMove | WindowFlags.NoTitleBar | WindowFlags.NoScrollbar;
        public DockPane DockPane { get; private set; }
        public List<DockableWindow> Children { get; } = new List<DockableWindow>();

        public static MainWindow Instance { get; private set; }
    }
}
