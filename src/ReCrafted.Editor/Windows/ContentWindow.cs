// ReCrafted Editor (c) 2016-2018 Always Too Late

using System;
using System.IO;
using System.Numerics;
using ImGuiNET;
using ReCrafted.Editor.Utilities;

namespace ReCrafted.Editor.Windows
{
    public class ContentWindow : WindowBase
    {
        private string _baseContentPath;

        private string[] _directories;
        private string[] _files;

        public override void Initialize()
        {
            _baseContentPath = PathUtils.NormalizePath(Path.Combine(Environment.CurrentDirectory, "../content"));
            CurrentDirectory = _baseContentPath;
            Refresh();
        }

        public override void Update()
        {
        }

        public override void Render()
        {
            if (ImGui.BeginMenuBar())
            {
                if (ImGui.BeginMenu("Create"))
                {
                    if (ImGui.MenuItem("Planet")) { }
                    if (ImGui.MenuItem("Asteroid")) { }
                    if (ImGui.MenuItem("Voxel Material")) { }
                    ImGui.EndMenu();
                }

                if (ImGui.MenuItem("Import"))
                {
                }
                if (ImGui.MenuItem("Refresh"))
                {
                    Refresh();
                }
                ImGui.EndMenuBar();
            }
            
            DrawContent();
        }

        public override void Dispose()
        {
        }
        
        private void DrawContent()
        {
            var position = ImGui.GetWindowPosition();
            var size = ImGui.GetWindowSize();
            var drawList = ImGui.GetWindowDrawList();
            
            ImGui.Columns(2, "##content_splitter", false);
            ImGui.SetColumnWidth(0, 150.0f);

            ImGui.BeginGroup();
            {
                ImGui.SmallButton("...");

                if (ImGui.IsLastItemActive() && ImGui.IsMouseClicked(0))
                {
                    Navigate(PathUtils.NormalizePath(Path.Combine(CurrentDirectory, "../")));
                }

                foreach (var dir in _directories)
                {
                    ImGui.Button(Path.GetFileNameWithoutExtension(dir));

                    if (ImGui.IsLastItemActive() && ImGui.IsMouseClicked(0))
                    {
                        Navigate(dir);
                    }
                }
            }
            ImGui.EndGroup();
            
            ImGui.NextColumn();
            ImGui.BeginGroup();
            {
                var width = ImGui.GetColumnWidth(1);
                var itemsPerLine = (int)Math.Floor(width / 64.0f);

                if (itemsPerLine == 0)
                    itemsPerLine = 1;
                
                var id = 1;
                foreach (var directory in _directories)
                {
                    var fileName = Path.GetFileNameWithoutExtension(directory);

                    ImGui.Button(fileName, new Vector2(64.0f, 64.0f));

                    if (ImGui.IsLastItemActive() && ImGui.IsMouseClicked(0))
                    {
                        Navigate(directory);
                    }

                    if (id % itemsPerLine != 0)
                        ImGui.SameLine();

                    id++;
                }

                foreach (var file in _files)
                {
                    var fileName = Path.GetFileNameWithoutExtension(file);

                    ImGui.Button(fileName, new Vector2(64.0f, 64.0f));

                    if (id % itemsPerLine != 0)
                        ImGui.SameLine();

                    id++;
                }
            }
            ImGui.EndGroup();
        }

        public void Refresh()
        {
            _directories = Directory.GetDirectories(CurrentDirectory);
            _files = Directory.GetFiles(CurrentDirectory);


        }

        public void Navigate(string directoryPath)
        {
            // TODO: Validate dir path
            CurrentDirectory = directoryPath;
            Refresh();
        }

        public string CurrentDirectory { get; private set; }

        public override string WindowName => "Content";
        public override WindowFlags WindowSettings => WindowFlags.NoCollapse | WindowFlags.MenuBar;
    }
}
