// ReCrafted Editor (c) 2016-2018 Always Too Late

using System;
using System.Diagnostics;
using System.IO;
using System.Numerics;
using System.Windows.Forms;
using DirectXTexNet;
using ImGuiNET;
using ReCrafted.Editor.Content.Importers;
using ReCrafted.Editor.Utilities;
using ReCrafted.Editor.Windows.Content.ContentTree;

namespace ReCrafted.Editor.Windows.Content
{
    public class ContentWindow : DockableWindow
    {
        private string _baseContentPath;
        private bool _firstDraw = true;

        private ContentTreeNode _contextDirectory;
        private ContentTreeFile _contextFile;

        public override void Initialize()
        {
            Current = this;
            _baseContentPath = PathUtils.NormalizePath(Path.Combine(Environment.CurrentDirectory, "../content"));
            Refresh();
        }
        
        public override void OnRender()
        {
            // Set this content window as current
            Current = this;
            
            DrawMenuBar();
            DrawContextMenus();
            DrawContent();
        }

        public override void Dispose()
        {
        }

        private void DrawMenuBar()
        {
            if (ImGui.BeginMenuBar())
            {
                if (ImGui.BeginMenu("Create"))
                {
                    if (ImGui.MenuItem("Voxel Material")) { }
                    ImGui.EndMenu();
                }

                if (ImGui.MenuItem("Import"))
                {
                    using (var openFileDialog = new OpenFileDialog())
                    {
                        openFileDialog.InitialDirectory = Environment.CurrentDirectory;
                        openFileDialog.Filter = @"All files (*.*)|*.*";
                        openFileDialog.FilterIndex = 0;
                        openFileDialog.RestoreDirectory = true;

                        if (openFileDialog.ShowDialog() == DialogResult.OK)
                        {
                            TryImport(openFileDialog.FileName);
                        }
                    }
                }

                if (ImGui.MenuItem("Export"))
                {
                }

                if (ImGui.MenuItem("Refresh"))
                {
                    Refresh();
                }
                ImGui.EndMenuBar();
            }
        }

        private void DrawContextMenus()
        {
            const string dirContextMenuId = "##content_popup_dir";
            const string fileContextMenuId = "##content_popup_file";

            // Push directory context menu
            if (ImGui.BeginPopup(dirContextMenuId))
            {
                Debug.Assert(_contextDirectory != null);

                if (ImGui.Selectable("Refresh"))
                {
                    _contextDirectory.Refresh();
                }
                if (ImGui.Selectable("Navigate"))
                {
                    Navigate(_contextDirectory);
                }
                ImGui.Separator();
                if (ImGui.Selectable("Delete"))
                {
                    _contextDirectory.Delete();
                    CurrentNode = _contextDirectory.Parent;
                }
                ImGui.EndPopup();
            }

            // Push file context menu
            if (ImGui.BeginPopup(fileContextMenuId))
            {
                Debug.Assert(_contextFile != null);

                if (ImGui.Selectable("Refresh Icon"))
                {
                    _contextFile.RefreshPreview();
                }
                ImGui.Separator();
                if (ImGui.Selectable("Delete"))
                {
                    _contextFile.Delete();
                }
                ImGui.EndPopup();
            }
        }

        private void DrawContent()
        {
            const string dirContextMenuId = "##content_popup_dir";
            const string fileContextMenuId = "##content_popup_file";
            
            ImGui.Columns(2, "##content_splitter", true);
            
            if (_firstDraw)
            {
                ImGui.SetColumnWidth(0, 250.0f);
                _firstDraw = false;
            }
            
            ImGui.PushStyleColor(ImGuiCol.Button, new Vector4(0.1f, 0.1f, 0.1f, 0.25f));
            ImGui.PushStyleColor(ImGuiCol.ButtonActive, new Vector4(0.1f, 0.1f, 0.1f, 0.38f));
            ImGui.PushStyleColor(ImGuiCol.ButtonHovered, new Vector4(1.0f, 0.1f, 0.1f, 0.42f));

            ImGui.BeginGroup();
            {
                var openContextMenu = false;

                // Render the directory
                RootNode.Render(directory =>
                {
                    openContextMenu = true;
                    _contextDirectory = directory;
                });

                // Open context menu when needed
                if (openContextMenu)
                {
                    ImGui.OpenPopup(dirContextMenuId);
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
                foreach (var directory in CurrentNode.SubDirectories)
                {
                    ImGui.Button(directory.Name, new Vector2(64.0f, 64.0f));

                    // Open context menu
                    if (ImGui.IsItemClicked(1))
                    {
                        _contextDirectory = directory;
                        ImGui.OpenPopup(dirContextMenuId);
                    }

                    // Navigate
                    if (ImGui.IsItemActive() && ImGui.IsMouseClicked(0))
                        Navigate(directory);

                    if (id % itemsPerLine != 0)
                        ImGui.SameLine();

                    // TODO: Directory icon

                    id++;
                }

                foreach (var file in CurrentNode.Files)
                {
                    ImGui.Button(file.Name, new Vector2(64.0f, 64.0f));

                    // Open context menu
                    if (ImGui.IsItemClicked(1))
                    {
                        _contextFile = file;
                        ImGui.OpenPopup(fileContextMenuId);
                    }

                    if (id % itemsPerLine != 0)
                        ImGui.SameLine();

                    // TODO: Item preview

                    id++;
                }

                // Open context menu when content surface gets click
                if (!ImGui.IsAnyItemHovered() && ImGui.IsMouseClicked(1))
                {
                    _contextDirectory = CurrentNode;
                    ImGui.OpenPopup(dirContextMenuId);
                }
            }
            ImGui.EndGroup();

            ImGui.PopStyleColor(3);
        }

        public void TryImport(string fileName)
        {
            switch (Path.GetExtension(fileName))
            {
                case ".bmp":
                case ".jpg":
                case ".png":
                case ".tga":
                case ".dds":
                case ".hdr":
                {
                    // TODO: Open texture importer window

                    var compressResult = MessageBox.Show("Compress this texture?", "Texture import.", MessageBoxButtons.YesNo);
                    var mipMapsResult = MessageBox.Show("Generate mip maps?", "Texture import.", MessageBoxButtons.YesNo);

                    // Temporary, import the texture
                    var outputFileName = Path.Combine(CurrentNode.Path, Path.GetFileNameWithoutExtension(fileName) + ".rcasset");
                    TextureImporter.Instance.ImportAsset(fileName, outputFileName, new TextureImporter.Settings
                    {
                        GenerateMipMaps = mipMapsResult == DialogResult.Yes,
                        Compress = compressResult == DialogResult.Yes,
                        CompressionFlags = TEX_COMPRESS_FLAGS.PARALLEL,
                        CompressionFormat = DXGI_FORMAT.BC7_UNORM
                    });

                    break;
                }
                // TODO: Add more import options
                default:
                    // TODO: Add log
                    break;
            }
        }

        public void Navigate(ContentTreeNode node)
        {
            CurrentNode = node;
        }

        public void Refresh()
        {
            // Create root node
            RootNode = new ContentTreeNode(null, _baseContentPath, "Content");
            RootNode.Refresh();

            // Set root as current
            CurrentNode = RootNode;

            // TODO: Load which nodes have been opened last time
        }
        
        public ContentTreeNode RootNode { get; private set; }
        public ContentTreeNode CurrentNode { get; private set; }

        public override string WindowName => "Content";
        public override ImGuiWindowFlags WindowSettings => ImGuiWindowFlags.NoCollapse | ImGuiWindowFlags.MenuBar;

        public static ContentWindow Current { get; private set; }
    }
}
