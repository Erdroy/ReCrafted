// ReCrafted Editor (c) 2016-2018 Always Too Late

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Numerics;
using ImGuiNET;
using Newtonsoft.Json;
using ReCrafted.Editor.Common;
using ReCrafted.Editor.Content;
using ReCrafted.Editor.Content.Assets;
using ReCrafted.Editor.Core;
using ReCrafted.Editor.Windows.Content;
using ReCrafted.Editor.Windows.Docking;
using ReCrafted.Editor.Windows.Graph;
using ReCrafted.Editor.Windows.Layout;

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
        
        public void Render()
        {
            if (ImGui.IsMouseReleased(0))
            {
                DockSplitter.IsAnySplitterDragging = false;
                DockableWindow.IsAnyWindowDragging = false;
            }

            ImGui.PushStyleVar(StyleVar.PopupBorderSize, 0.0f);

            var opened = true;
            var windowRect = WindowRect;

            ImGui.BeginWindow("Main", ref opened, new Vector2(windowRect.Width, windowRect.Height), 0.0f, WindowSettings);

            // Update DockPane
            DockPane.Update();
            
            // TODO: Refactor main menu
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

                if (ImGui.BeginMenu("View"))
                {
                    if (ImGui.MenuItem("Console Window"))
                    {
                    }

                    if (ImGui.MenuItem("Content Window"))
                    {
                    }

                    if (ImGui.MenuItem("Voxel Editor Window"))
                    {
                    }

                    ImGui.Separator();

                    if (ImGui.MenuItem("Save layout"))
                    {
                        SaveLayout();
                    }

                    if (ImGui.MenuItem("Load layout"))
                    {
                        LoadLayout();
                    }

                    if (ImGui.MenuItem("Reset layout"))
                    {
                        ResetLayout();
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
                        asset.Unload();
                    }

                    ImGui.EndMenu();
                }

                if (ImGui.BeginMenu("Help"))
                {
                    if (ImGui.MenuItem("Open Homepage"))
                        Process.Start("https://recraftedgame.com/");

                    if (ImGui.MenuItem("Open Documentation"))
                        Process.Start("https://editor.recraftedgame.com/docs/");

                    if (ImGui.MenuItem("Report Issue"))
                        Process.Start("https://editor.recraftedgame.com/bug-report/");

                    ImGui.EndMenu();
                }

                ImGui.EndMainMenuBar();
            }
            
            var childrenToRemove = new List<DockableWindow>();

            // Render children windows
            foreach (var child in Children)
            {
                if(child.WindowVisible)
                    child.Render();

                if(WindowClosed)
                    childrenToRemove.Add(child);
            }

            // Remove children windows when needed
            foreach (var child in childrenToRemove)
                Children.Remove(child);
            childrenToRemove.Clear();

            ImGui.PopStyleVar();

            ImGui.EndWindow();
        }

        public void SaveLayout()
        {
            // Serialize layout
            var layout = SerializeLayout();

            // Write layout info
            var json = JsonConvert.SerializeObject(layout, Formatting.Indented);
            File.WriteAllText("layout.json", json);
        }

        public void LoadLayout()
        {
            // Clear layout before loading
            ClearLayout();

            // Read layout info
            var json = File.ReadAllText("layout.json");
            var layout = JsonConvert.DeserializeObject<LayoutDescription>(json);

            // Deserialize layout
            DeserializeLayout(layout);
        }

        public void ResetLayout()
        {
            // Clear layout before reset
            ClearLayout();

            // Create content window
            var content = DockPane.Dock(AddChildren<ContentWindow>());
            var graph = content.Dock(AddChildren<GraphWindowBase>(), DockType.Horizontal, DockDirection.Up);
            var content2 = graph.Dock(AddChildren<ContentWindow>(), DockType.Vertical, DockDirection.Left);

            // Add console window
            content2.Dock(AddChildren<ConsoleWindow>(), DockType.Vertical, DockDirection.Left);

            // Recalculate layout
            DockPane.RecalculateLayout();
        }

        public void ClearLayout()
        {
            foreach (var window in Children)
            {
                window.Close();
                window.Dispose();
            }

            Children.Clear();
            DockPane.Root = null;
        }

        public void Dispose()
        {
            foreach (var child in Children)
            {
                child.Close();
                child.Dispose();
            }
        }

        public void Open()
        {
            throw new NotImplementedException();
        }

        public void Hide()
        {
            throw new NotImplementedException();
        }

        public void Close()
        {
            throw new NotImplementedException();
        }

        public LayoutDescription SerializeLayout()
        {
            var layout = new LayoutDescription
            {
                Windows = new LayoutDescription.Window[Children.Count]
            };

            // Write all windows
            for (var i = 0; i < Children.Count; i++)
            {
                var window = Children[i];
                
                layout.Windows[i] = new LayoutDescription.Window
                {
                    Id = window.WindowId,
                    Type = window.GetType().FullName,
                    Floating = window.Floating,
                    X = window.Rect.X,
                    Y = window.Rect.Y,
                    Width = window.Rect.Width,
                    Height = window.Rect.Height,
                };
            }

            if (DockPane.Root != null)
            {
                // Root always contains only ChildA
                var root = layout.Layout = new LayoutDescription.LayoutInfo
                {
                    DockType = DockType.Fill,
                    DockSize = 0.5f
                };

                var rootSplitter = (DockSplitter) DockPane.Root;

                if (rootSplitter.ChildA is DockSplitter)
                {
                    // Iterate
                    BuildLayoutInfo((DockSplitter)rootSplitter.ChildA, root);
                }
            }

            return layout;
        }

        private void BuildLayoutInfo(DockSplitter splitter, LayoutDescription.LayoutInfo parent)
        {
            var currentLayout = parent.Children = new LayoutDescription.LayoutInfo
            {
                DockType = splitter.DockType,
                DockSize = splitter.Size
            };

            var current = parent.Children;

            if (splitter.ChildA != null)
            {
                if (splitter.ChildA is DockSplitter)
                {
                    BuildLayoutInfo((DockSplitter) splitter.ChildA, currentLayout);
                }
                else
                {
                    var window = (IWindow) splitter.ChildA;
                    current.WindowAId = window.WindowId;
                }
            }

            if (splitter.ChildB != null)
            {
                if (splitter.ChildB is DockSplitter)
                {
                    BuildLayoutInfo((DockSplitter)splitter.ChildB, currentLayout);
                }
                else
                {
                    var window = (IWindow)splitter.ChildB;
                    current.WindowBId = window.WindowId;
                }
            }
        }

        public void DeserializeLayout(LayoutDescription layout)
        {
            // Clear
            Children.Clear();
            DockPane.Root = null;

            if (layout.Windows == null)
                return;

            // Create windows
            foreach (var windowDesc in layout.Windows)
            {
                var windowType = Type.GetType(windowDesc.Type);

                if (windowType == null)
                {
                    Logger.LogError($"Failed to deserialize whole layout. Missing window type: '{windowDesc.Type}'!");
                    continue;
                }

                // Create window instance and setup it's values
                var window = (DockableWindow)Activator.CreateInstance(windowType);

                window.Floating = windowDesc.Floating;
                window.WindowId = windowDesc.Id;

                if (windowDesc.Floating)
                    window.Rect = new Rectangle(windowDesc.X, windowDesc.Y, windowDesc.Width, windowDesc.Height);

                // Initialize
                window.Initialize();
                window.Open();
                window.Focus();
                Children.Add(window);
            }
            
            // Apply layout
            var parentLayout = layout.Layout;
            if (parentLayout != null)
            {
                Debug.Assert(parentLayout.DockType == DockType.Fill);
                
                // Dock root window
                var panel = DockPane.Dock(Children.First(x => x.WindowId == parentLayout.Children.WindowBId));
                DockPane.RecalculateLayout();
                parentLayout = parentLayout.Children;

                // Totally temporary, but works.
                while (panel != null)
                {
                    if (parentLayout.Children != null)
                    {
                        var windowB = Children.FirstOrDefault(x => x.WindowId == parentLayout.WindowBId);
                        panel = panel.Dock(windowB, parentLayout.DockType, DockDirection.Right, parentLayout.DockSize);
                        DockPane.RecalculateLayout();
                        parentLayout = parentLayout.Children;
                    }
                    else
                    {
                        var windowA = Children.FirstOrDefault(x => x.WindowId == parentLayout.WindowAId);
                        Children.Remove(windowA);
                        //panel = panel.Dock(windowA, parentLayout.DockType, DockDirection.Right, parentLayout.DockSize);

                        //var windowB = Children.FirstOrDefault(x => x.WindowId == parentLayout.WindowBId);
                        //panel.Dock(windowB, parentLayout.DockType, DockDirection.Right, parentLayout.DockSize);

                        panel = null;
                    }
                }
            }

            DockPane.RecalculateLayout();
        }
        
        public T GetOrAddChildren<T>() where T : DockableWindow, new()
        {
            var found = Children.First(x => x is T);
            if (found != null)
                return (T)found;

            var window = new T
            {
                WindowId = _lastWindowId++
            };

            window.Initialize();
            window.Open();
            window.Focus();
            Children.Add(window);
            return window;
        }

        public T AddChildren<T>() where T : DockableWindow, new()
        {
            var window = new T
            {
                WindowId = _lastWindowId++
            };

            window.Initialize();
            window.Open();
            window.Focus();
            Children.Add(window);
            return window;
        }

        public bool WindowClosed { get; } = false;
        public bool WindowVisible { get; } = true;
        public int WindowId => 0;
        public Rectangle WindowRect => new Rectangle(0, 20, EditorApplication.Current.SdlWindow.Width, EditorApplication.Current.SdlWindow.Height - 20);
        public string WindowName => "MainWindow";
        public WindowFlags WindowSettings => WindowFlags.NoResize | WindowFlags.NoInputs | WindowFlags.NoMove | WindowFlags.NoTitleBar | WindowFlags.NoScrollbar | WindowFlags.NoSavedSettings;

        public DockPane DockPane { get; private set; }
        public List<DockableWindow> Children { get; } = new List<DockableWindow>();

        public static MainWindow Instance { get; private set; }
    }
}
