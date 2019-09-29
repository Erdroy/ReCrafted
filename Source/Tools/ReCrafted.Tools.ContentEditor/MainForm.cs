// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using System;
using System.IO;
using System.Linq;
using System.Windows.Forms;
using ReCrafted.Tools.ContentEditor.Properties;

namespace ReCrafted.Tools.ContentEditor
{
    /// <summary>
    ///     Main form of the whole application.
    /// </summary>
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();
        }

        private void Form_OnLoaded(object sender, EventArgs e)
        {
            // Load settings
            Settings.Current = Settings.FromFile(Settings.FileName);

            // The application is now fully loaded!
        }

        private void Form_OnShown(object sender, EventArgs e)
        {
            // Ask for the game directory when running this application the first time.
            if (string.IsNullOrEmpty(Settings.Current.GameDirectory))
            {
                SelectDirectory();
            }
        }

        private void Exit_OnClick(object sender, EventArgs e)
        {
            // Exit the application!
            Application.Exit();
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
    }
}
