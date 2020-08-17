// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

using System;
using System.Collections;
using System.Drawing;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Windows.Forms;
using ReCrafted.Tools.ContentEditor.Properties;

namespace ReCrafted.Tools.ContentEditor.Content.Common
{
    public partial class AssetFinder : Form
    {
        public const int MaxItems = 64;

        public AssetFinder()
        {
            CurrentFilter = "t:*;n:*";
            InitializeComponent();
        }

        private void AssetFinder_Load(object sender, System.EventArgs e)
        {
            AssetFilter.Text = CurrentFilter;
            SetupContentView();
            RefreshAssets();
        }

        private void AssetFinder_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (DialogResult != DialogResult.OK)
                DialogResult = DialogResult.Abort;
        }

        private void AssetFinder_Resized(object sender, System.EventArgs e)
        {
            RefreshAssets();
        }

        private void TextBox1_TextChanged(object sender, System.EventArgs e)
        {
            CurrentFilter = AssetFilter.Text;
            RefreshAssets();
        }

        private void ContentView_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            var senderList = (ListView)sender;
            var clickedItem = senderList.HitTest(e.Location).Item;

            if (clickedItem != null)
            {
                if (clickedItem.Tag is AssetCache.AssetCacheItem asset)
                {
                    SelectedAsset = asset;
                    DialogResult = DialogResult.OK;
                    Close();
                }
            }
        }

        private void RefreshAssets()
        {
            var filterParts = CurrentFilter.Split(new[] {";", ","}, StringSplitOptions.RemoveEmptyEntries);

            var nameFilter = filterParts.FirstOrDefault(x => x.StartsWith("n:", StringComparison.InvariantCultureIgnoreCase));
            if (!string.IsNullOrEmpty(nameFilter))
                nameFilter = nameFilter.Replace("n:", "");

            var typeFilter = filterParts.FirstOrDefault(x => x.StartsWith("t:", StringComparison.InvariantCultureIgnoreCase));
            if (!string.IsNullOrEmpty(typeFilter))
                typeFilter = typeFilter.Replace("t:", "");

            ContentView.Items.Clear();

            var i = 0;
            foreach (var asset in AssetCache.Assets)
            {
                if (i >= MaxItems)
                    break;

                var assetName = Path.GetFileNameWithoutExtension(asset.AssetFile);
                var assetType = asset.AssetType.ToString();

                if (assetName != null && !string.IsNullOrEmpty(nameFilter) && nameFilter != "*")
                {
                    if (assetName.IndexOf(nameFilter, StringComparison.InvariantCultureIgnoreCase) == -1)
                        continue;
                }

                if (string.IsNullOrEmpty(ForceType))
                {
                    if (!string.IsNullOrEmpty(typeFilter) && typeFilter != "*")
                    {
                        if (assetType.IndexOf(typeFilter, StringComparison.InvariantCultureIgnoreCase) == -1)
                            continue;
                    }
                }
                else
                {
                    if (assetType.IndexOf(ForceType, StringComparison.InvariantCultureIgnoreCase) == -1)
                        continue;
                }

                var imageKey = PreviewIconManager.Instance.GenerateOrLoad(asset.AssetFile, () =>
                {
                    ContentView.Refresh();
                }, true);

                ContentView.Items.Add(new ListViewItem
                {
                    Text = assetName,
                    Tag = asset,
                    ImageKey = imageKey
                });

                i++;
            }
        }

        private void SetupContentView()
        {
            ContentViewImages = new ImageList
            {
                ImageSize = new Size(64, 64),
                ColorDepth = ColorDepth.Depth32Bit
            };

            // Setup content images
            ContentView.LargeImageList = PreviewIconManager.Instance.ContentViewImages;
        }

        public AssetCache.AssetCacheItem SelectedAsset { get; private set; }
        public string CurrentFilter { get; set; }
        public string ForceType { get; set; }
        public ImageList ContentViewImages { get; set; }
    }
}
