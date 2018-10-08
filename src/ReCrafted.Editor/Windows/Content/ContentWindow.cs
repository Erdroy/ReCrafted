// ReCrafted Editor (c) 2016-2018 Always Too Late

using System;
using System.IO;
using System.Numerics;
using System.Windows.Forms;
using DirectXTexNet;
using ImGuiNET;
using ReCrafted.Editor.Common;
using ReCrafted.Editor.Content.Importers;
using ReCrafted.Editor.Utilities;
using ReCrafted.Editor.Windows.Content.ContentTree;

namespace ReCrafted.Editor.Windows.Content
{
    public class ContentWindow : DockableWindow
    {
        private string _baseContentPath;
        private bool _firstDraw = true;

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
            
            DrawContent();
        }

        public override void Dispose()
        {
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
                    var outputFileName = "../content/" + Path.GetFileNameWithoutExtension(fileName) + ".rcasset";
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
        
        private void DrawContent()
        {
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
                // Render the directory
                RootNode.Render();
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

                    if (ImGui.IsItemActive() && ImGui.IsMouseClicked(0))
                    {
                        Navigate(directory);
                    }

                    if (id % itemsPerLine != 0)
                        ImGui.SameLine();

                    // TODO: Directory icon

                    id++;
                }

                foreach (var file in CurrentNode.Files)
                {
                    ImGui.Button(file.Name, new Vector2(64.0f, 64.0f));

                    if (id % itemsPerLine != 0)
                        ImGui.SameLine();

                    // TODO: Item preview

                    id++;
                }
            }
            ImGui.EndGroup();

            ImGui.PopStyleColor(3);
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
