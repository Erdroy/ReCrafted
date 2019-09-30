// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using System;
using System.Collections;
using System.Drawing;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Windows.Forms;
using ReCrafted.Tools.ContentEditor.Content;
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

        public MainForm()
        {
            InitializeComponent();
        }

        private void Form_OnShown(object sender, EventArgs e)
        {
            // Load settings
            Settings.Current = Settings.FromFile(Settings.FileName);

            // Ask for the game directory when running this application the first time.
            if (string.IsNullOrEmpty(Settings.Current.GameDirectory))
                SelectDirectory();

            var contentRootPath = Path.Combine(Settings.Current.GameDirectory, "Content");

            // Setup content images
            SetupContentView();

            // Create content browser
            Browser = new ContentBrowser(contentRootPath, ContentTree, ContentView)
            {
                CurrentPath = contentRootPath
            };

            // The application is now fully loaded!
        }

        private void SetupContentView()
        {
            ContentViewImages = new ImageList
            {
                ImageSize = new Size(64, 64),
                ColorDepth = ColorDepth.Depth32Bit
            };

            var resourceSet = Resources.ResourceManager.GetResourceSet(CultureInfo.InvariantCulture, true, false);
            if (resourceSet != null)
            {
                foreach (DictionaryEntry entry in resourceSet)
                {
                    if (entry.Value is Bitmap value)
                    {
                        ContentViewImages.Images.Add((string)entry.Key, value);
                    }
                }
            }

            // Setup image lists
            ContentView.LargeImageList = ContentViewImages;
        }

        private void ContentView_OnDrop(object sender, DragEventArgs e)
        {
            var files = (string[])e.Data.GetData(DataFormats.FileDrop);

            foreach (var file in files)
            {
                // Import asset
                ImportAsset(file, Browser.CurrentPath);
            }
        }

        private void ContentView_OnEnter(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
                e.Effect = DragDropEffects.Copy;
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
            }
        }

        private void Exit_OnClick(object sender, EventArgs e)
        {
            // Exit the application!
            Application.Exit();
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
                if ((metadata = importer.ShowImportDialog(sourceFile, sourceFileMeta)) == null)
                    return;
            }
            else
            {
                // Just load the metadata file using importer
                metadata = importer.LoadMetadata(sourceFileMeta);
            }

            // Import asset using metadata
            if (!importer.ImportAsset(sourceFile, targetDirectory, metadata))
            {
                MessageBox.Show(@"Error", string.Format(Resources.FailedToImport, sourceFile));
            }
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
        ///     The list of all images used by the ContentView.
        /// </summary>
        public ImageList ContentViewImages { get; private set; }

        /// <summary>
        ///     The <see cref="ContentBrowser"/> class instance.
        /// </summary>
        public ContentBrowser Browser { get; private set; }
    }
}
