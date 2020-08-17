// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

using System;
using System.Windows.Forms;
using ReCrafted.Tools.ContentEditor.Content;
using ReCrafted.Tools.ContentEditor.Content.Common;

namespace ReCrafted.Tools.ContentEditor.Controls
{
    public partial class TextureSelect : UserControl
    {
        public TextureSelect()
        {
            InitializeComponent();
        }

        private void BrowseTexture(object sender, EventArgs e)
        {
            var finder = new AssetFinder
            {
                ForceType = "Texture2D",
                CurrentFilter = "n:*"
            };

            if (finder.ShowDialog() == DialogResult.OK)
            {
                if (finder.SelectedAsset.AssetType == AssetType.Texture2D)
                {
                    SelectedAsset = finder.SelectedAsset;

                    // Update icon
                    var imageKey = PreviewIconManager.Instance.GenerateOrLoad(SelectedAsset.AssetFile, () => { }, true);
                    Preview.Image = PreviewIconManager.Instance.ContentViewImages.Images[imageKey];
                }
            }
        }

        public AssetCache.AssetCacheItem SelectedAsset;
    }
}
