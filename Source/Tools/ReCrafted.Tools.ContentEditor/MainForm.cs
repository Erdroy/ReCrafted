// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

using System;
using System.IO;
using System.Linq;
using System.Windows.Forms;
using ReCrafted.Tools.ContentEditor.Content;
using ReCrafted.Tools.ContentEditor.Content.Assets;
using ReCrafted.Tools.ContentEditor.Content.Editors;
using ReCrafted.Tools.ContentEditor.Content.Importers;
using ReCrafted.Tools.ContentEditor.Properties;

namespace ReCrafted.Tools.ContentEditor
{
    /// <summary>
    ///     Main form of the whole application.
    /// </summary>
    public partial class MainForm : Form
    {
        private static readonly AssetImporter[] Importers = {
            new TextureImporter()
        };

        private PreviewIconManager _previewIconManager;

        public MainForm()
        {
            InitializeComponent();
        }

        private void Form_OnShown(object sender, EventArgs e)
        {
            // Set instance
            Instance = this;

            // Load settings
            Settings.Current = Settings.FromFile(Settings.FileName);

            // Create and show progress form
            var progressImport = new ProgressForm
            {
                Text = Resources.UpdateingAssetCache
            };
            progressImport.Show(Instance);
            {
                // Rebuild cache by default
                AssetCache.RebuildCache();
                AssetCache.SaveCache();
            }
            // Close progress form
            progressImport.Close();

            // Ask for the game directory when running this application the first time.
            if (string.IsNullOrEmpty(Settings.Current.GameDirectory))
                SelectDirectory();

            var contentRootPath = Path.Combine(Settings.Current.GameDirectory, "Content");

            // Setup and run preview icon manager
            _previewIconManager = new PreviewIconManager();
            _previewIconManager.Run();

            // Setup content images
            ContentView.LargeImageList = _previewIconManager.ContentViewImages;

            // Create content browser
            Browser = new ContentBrowser(contentRootPath, ContentTree, ContentView)
            {
                CurrentPath = contentRootPath
            };

            // The application is now fully loaded!
        }

        private void Form_Closed(object sender, FormClosedEventArgs e)
        {
            _previewIconManager.Dispose();
        }

        private void ContentView_OnDrop(object sender, DragEventArgs e)
        {
            var files = (string[])e.Data.GetData(DataFormats.FileDrop);

            foreach (var file in files)
            {
                // Import asset
                ImportAsset(file, Browser.CurrentPath);
            }
            Browser.Refresh();
        }

        private void ContentView_OnEnter(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
                e.Effect = DragDropEffects.Copy;
        }

        private void ContentView_OnKeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyData == Keys.F2 && ContentView.SelectedItems.Count > 0)
            {
                //ContentView.SelectedItems[0].BeginEdit();
                // TODO: Handle label edit
                // TODO: Handle duplicated names
            }
        }

        private void Import_OnClick(object sender, EventArgs e)
        {
            var openFileDialog = new OpenFileDialog
            {
                Title = Resources.SelectImportFile
            };

            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                // Import asset
                ImportAsset(openFileDialog.FileName, Browser.CurrentPath);
                Browser.Refresh();
            }
        }

        private void Exit_OnClick(object sender, EventArgs e)
        {
            // Exit the application!
            Application.Exit();
        }

        private void CreateVoxelObject(object sender, EventArgs e)
        {
            Browser.CreateItem(fileName =>
            {
                var objectEditor = new VoxelObjectEditor();
                var result = objectEditor.ShowDialog();

                if (result == DialogResult.OK)
                {
                    // Save file with given name
                    using (var fs = File.Create(Path.Combine(Browser.CurrentPath, fileName)))
                    {
                        var asset = new VoxelObjectAsset
                        {
                            AssetGuid = Guid.NewGuid(),
                            ObjectName = objectEditor.VoxelObjectName,

                            // Planet (VoxelObjectType.Planet)
                            // TODO: Object type selection (CHM or PreGen) PreGen is suitable for VoxelObject with total size < 32MB (small planetoids, asteroids etc.)
                            ObjectType = 1,

                            // Temporary settings
                            HillsHeight = 100,
                            InitialOctreeDepth = 2,
                            MaximalSurfaceHeight = 1024,
                            MinimalSurfaceHeight = 924,

                            // Set textures (CHM only)
                            TopHeightMap = Path.Combine(Browser.BasePath, objectEditor.TopHeightMap),
                            BottomHeightMap = Path.Combine(Browser.BasePath, objectEditor.BottomHeightMap),
                            LeftHeightMap = Path.Combine(Browser.BasePath, objectEditor.LeftHeightMap),
                            RightHeightMap = Path.Combine(Browser.BasePath, objectEditor.RightHeightMap),
                            FrontHeightMap = Path.Combine(Browser.BasePath, objectEditor.FrontHeightMap),
                            BackHeightMap = Path.Combine(Browser.BasePath, objectEditor.BackHeightMap)
                        };

                        // Serialize this asset into the file
                        asset.Serialize(fs);

                        // Add asset to the cache
                        AssetCache.AddAssetType(fileName, asset.AssetGuid, asset.AssetType);
                    }

                    return true;
                }

                // Well, cancel.
                return false;
            });
        }

        private void CreateVoxelMaterial(object sender, EventArgs e)
        {
            Browser.CreateItem(fileName =>
            {
                // Open voxel material edit window
                // Note: This window should be also usable for editing existing materials.
                var materialEditor = new VoxelMaterialEditor();
                var result = materialEditor.ShowDialog();

                if (result == DialogResult.OK)
                {
                    // Save file with given name
                    using (var fs = File.Create(Path.Combine(Browser.CurrentPath, fileName)))
                    {
                        var asset = new VoxelMaterialAsset
                        {
                            AssetGuid = Guid.NewGuid(),
                            VoxelName = Path.GetFileNameWithoutExtension(fileName),
                            VoxelHardness = (byte)materialEditor.Hardness,
                            VoxelMaterial = 0, // Voxel Material will be calculated during game's runtime
                        };

                        // Serialize this asset into the file
                        asset.Serialize(fs);

                        // Add asset to the cache
                        AssetCache.AddAssetType(fileName, asset.AssetGuid, asset.AssetType);
                    }

                    return true;
                }

                // Well, cancel.
                return false;
            });
        }

        private static void ImportAsset(string sourceFile, string targetDirectory)
        {
            var fileInfo = new FileInfo(sourceFile);

            if (!fileInfo.Exists)
            {
                MessageBox.Show(@"Error", string.Format(Resources.ImportFileNotFound, sourceFile));
                return;
            }

            // Get source metadata file path and the source file extension
            var sourceFileMeta = $"{sourceFile}.rcmeta";
            var sourceFileExtension = fileInfo.Extension;
            var targetFile = Path.Combine(targetDirectory, Path.GetFileNameWithoutExtension(sourceFile) + ".rcasset");

            // Get the importer that supports the source file's extension.
            var importer = Importers.FirstOrDefault(x => x.SupportedExtensions.Contains(sourceFileExtension));

            if (importer == null)
            {
                MessageBox.Show(@"Error", string.Format(Resources.FileNotSupported, sourceFile));
                return;
            }

            // Check metadata file [and if found, load metadata]
            IAssetMetadata metadata;
            if (!File.Exists(sourceFileMeta))
            {
                // If not found, show import asset window. If aborted, safely stop the import process.
                if ((metadata = importer.ShowImportDialog(sourceFile, sourceFileMeta, targetDirectory)) == null)
                    return;
            }
            else
            {
                // Just load the metadata file using importer
                metadata = importer.LoadMetadata(sourceFileMeta);
            }

            // Create and show progress form
            var progressImport = new ProgressForm();
            progressImport.Show(Instance);

            // Import asset using metadata
            if (!importer.ImportAsset(sourceFile, targetDirectory, metadata,
                (progress, text) => {
                    progressImport.Progress = progress;
                    progressImport.Text = text;
                    Application.DoEvents();
                }))
            {
                MessageBox.Show(@"Error", string.Format(Resources.FailedToImport, sourceFile));
            }

            // Add asset to cache
            AssetCache.AddAssetType(targetFile, metadata.AssetGuid, metadata.AssetType);

            // Close progress form
            progressImport.Close();
        }

        private static void SelectDirectory()
        {
            var openDialog = new OpenFileDialog
            {
                FileName = "ReCrafted.exe",
                CheckFileExists = true,
                ReadOnlyChecked = true,
                CheckPathExists = true,
                ShowReadOnly = false,
                Multiselect = false,
                Title = Resources.InitialSelectContentDirectory
            };

            while (true)
            {
                // Reset and show the dialog (again)
                openDialog.Reset();
                var result = openDialog.ShowDialog();

                // Check the results
                if (result == DialogResult.OK)
                {
                    // Ok, we've got it. Just check few more things, are the user is ready to go.
                    var fileInfo = new FileInfo(openDialog.FileName);

                    if (fileInfo.Name != @"ReCrafted.exe")
                        continue;

                    var binPath = fileInfo.DirectoryName;

                    // Get the game dir.
                    var gameDirectory = Path.Combine(binPath ?? throw new InvalidOperationException(), "..");

                    // Check if there is Content folder, if so, we're done here. Just save the settings file and everything is ready.
                    var gameDirectoryInfo = new DirectoryInfo(gameDirectory);
                    if (gameDirectoryInfo.GetDirectories().All(x => x.Name != "Content"))
                    {
                        MessageBox.Show(Resources.Error, Resources.InvalidGamePath);
                        break;
                    }

                    // Set GameDirectory
                    Settings.Current.GameDirectory = Path.GetFullPath(gameDirectory);
                    Settings.Current.ToFile(Settings.FileName);
                    return;
                }

                if (result == DialogResult.Cancel || result == DialogResult.Abort)
                    break;
            }

            // Failed, exit the application.
            Application.Exit();
        }

        /// <summary>
        ///     The <see cref="ContentBrowser"/> class instance.
        /// </summary>
        public ContentBrowser Browser { get; private set; }

        /// <summary>
        ///     The instance of the <see cref="MainForm"/> window class.
        /// </summary>
        public static MainForm Instance { get; private set; }
    }
}
