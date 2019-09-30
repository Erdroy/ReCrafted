// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using System.Windows.Forms;
using Newtonsoft.Json;
using ReCrafted.Tools.Common;

namespace ReCrafted.Tools.ContentEditor.Content.Importers
{
    /// <inheritdoc cref="AssetImporter"/>
    /// <summary>
    ///     Texture importer class.
    /// </summary>
    public class TextureImporter : AssetImporter
    {
        public class TextureMetadata : JsonTemplate<TextureMetadata>, IAssetMetadata
        {
            public bool GenerateMipmaps;
            public bool CompressTexture;
        }

        /// <inheritdoc cref="AssetImporter.LoadMetadata"/>
        public override IAssetMetadata LoadMetadata(string sourceFileMeta)
        {
            return TextureMetadata.FromFile(sourceFileMeta);
        }

        /// <inheritdoc cref="AssetImporter.ShowImportDialog"/>
        public override IAssetMetadata ShowImportDialog(string sourceFile, string sourceFileMeta, bool forceMetaFile = false)
        {
            var dialog = new TextureImportWindow
            {
                CreateMetaFile = forceMetaFile
            };

            // Show texture import dialog window
            var result = dialog.ShowDialog();

            // Check dialog result, should be OK
            if (result != DialogResult.OK)
                return null;

            // Create metadata object
            var metadata = new TextureMetadata
            {
                GenerateMipmaps = dialog.GenerateMipmaps,
                CompressTexture = dialog.CompressTexture
            };

            // Save metadata file if needed or forced
            if (dialog.CreateMetaFile || forceMetaFile)
            {
                // Save file
                metadata.ToFile(sourceFileMeta, Formatting.Indented);
            }

            return metadata;
        }

        /// <inheritdoc cref="AssetImporter.ImportAsset"/>
        public override bool ImportAsset(string sourceFile, string targetDirectory, IAssetMetadata metadata)
        {
            var textureMetadata = (TextureMetadata)metadata;

            // TODO: Proper file importing

            return false;
        }

        /// <inheritdoc cref="AssetImporter.SupportedExtensions"/>
        public override string[] SupportedExtensions => new[]
        {
            ".bmp", ".jpg", ".png", ".tga", ".hdr", ".dds"
        };
    }
}
