// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using DirectXTexNet;
using Newtonsoft.Json;
using ReCrafted.Tools.Common;
using ReCrafted.Tools.ContentEditor.Content.Assets;

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
            public string TargetDirectory { get; set; }
            public AssetType AssetType { get; set; }
            public Guid AssetGuid { get; set; }

            public bool GenerateMipmaps;
            public bool CompressTexture;
        }

        /// <inheritdoc cref="AssetImporter.LoadMetadata"/>
        public override IAssetMetadata LoadMetadata(string sourceFileMeta)
        {
            return TextureMetadata.FromFile(sourceFileMeta);
        }

        /// <inheritdoc cref="AssetImporter.ShowImportDialog"/>
        public override IAssetMetadata ShowImportDialog(string sourceFile, string sourceFileMeta, string targetDirectory, bool forceMetaFile = false)
        {
            var dialog = new TextureImportWindow
            {
                CreateMetaFile = forceMetaFile,
                Text = $@"Import '{sourceFile}'"
            };

            // Show texture import dialog window
            var result = dialog.ShowDialog();

            // Check dialog result, should be OK
            if (result != DialogResult.OK)
                return null;

            // Create metadata object
            var metadata = new TextureMetadata
            {
                TargetDirectory = targetDirectory,
                AssetType = AssetType.Texture2D,
                AssetGuid = Guid.NewGuid(),
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
        public override bool ImportAsset(string sourceFile, string targetDirectory, IAssetMetadata metadata, Action<float, string> importProgress = null)
        {
            var targetFile = Path.Combine(targetDirectory, Path.GetFileNameWithoutExtension(sourceFile) + ".rcasset");

            var textureMetadata = (TextureMetadata)metadata;

            // Load texture image
            var image = LoadImage(sourceFile);

            // Update progress
            importProgress?.Invoke(0.25f, "Importing texture");

            if (image == null)
                return false;

            // Get mip count
            var mipCount = image.GetImageCount();

            if (mipCount == 1 && textureMetadata.GenerateMipmaps)
            {
                // Generate mip maps
                var mipMaps = image.GenerateMipMaps(TEX_FILTER_FLAGS.DEFAULT, 0);

                if (mipMaps == null)
                    return false;

                // Set image 
                image.Dispose();
                image = mipMaps;

                mipCount = image.GetImageCount();
            }

            // Update progress
            importProgress?.Invoke(0.5f, "Importing texture");

            if (textureMetadata.CompressTexture)
            {
                // Threshold is 0.5, because it is being used only by BC1. Maybe expose if needed
                var compressedImage = image.Compress(DXGI_FORMAT.BC7_UNORM, TEX_COMPRESS_FLAGS.BC7_QUICK | TEX_COMPRESS_FLAGS.PARALLEL, 0.5f);

                // Set image
                image.Dispose();
                image = compressedImage;
            }

            // Update progress
            importProgress?.Invoke(0.8f, "Importing texture");

            // Get first mip map
            var mip0 = image.GetImage(0);

            // Create asset
            var asset = new TextureAsset
            {
                Format = mip0.Format,
                MipCount = mipCount,
                Mips = new TextureAsset.Mip[mipCount],
                AssetGuid = metadata.AssetGuid
            };

            // Set texture mip maps
            var textureSize = 0u;
            for (var i = 0; i < mipCount; i++)
            {
                var img = image.GetImage(i);

                var mip = asset.Mips[i] = new TextureAsset.Mip
                {
                    Width = img.Width,
                    Height = img.Height,
                    Pitch = (int)img.RowPitch,
                    Size = (int)img.SlicePitch // Slice pitch is size of single slice, so whole single mip
                };

                textureSize += (uint)mip.Size;

                mip.Data = new byte[mip.Size];
                Marshal.Copy(img.Pixels, mip.Data, 0, mip.Size);
            }

            // Total size
            asset.TotalSize = textureSize;

            // Update progress
            importProgress?.Invoke(1.0f, "Importing texture");

            // Serialize asset
            using (var fs = new FileStream(targetFile, FileMode.Create, FileAccess.Write))
            {
                asset.Serialize(fs);
            }

            // Dispose image
            image.Dispose();

            // Generate preview and save it in './Cache/' directory
            //var previewImage = image.CreateImageCopy(0, false, CP_FLAGS.BPP24);
            //previewImage.Resize(0, 64, 64, TEX_FILTER_FLAGS.POINT);
            var previewImage = LoadImage(sourceFile);
            var scaledPreviewImage = previewImage.Resize(64, 64, TEX_FILTER_FLAGS.FANT);
            scaledPreviewImage.SaveToWICFile(0, WIC_FLAGS.NONE, new Guid(0x19e4a5aa, 0x5662, 0x4fc5, 0xa0, 0xc0, 0x17, 0x58, 0x2, 0x8e, 0x10, 0x57), $"./Cache/{asset.AssetGuid}.jpg");

            // Dispose image
            previewImage.Dispose();
            scaledPreviewImage.Dispose();

            return true;
        }

        private static ScratchImage LoadImage(string file)
        {
            var extension = Path.GetExtension(file)?.ToLower();

            switch (extension)
            {
                case ".bmp":
                case ".jpg":
                case ".png":
                    return TexHelper.Instance.LoadFromWICFile(file, WIC_FLAGS.IGNORE_SRGB);

                case ".tga":
                    return TexHelper.Instance.LoadFromTGAFile(file);

                case ".hdr":
                    return TexHelper.Instance.LoadFromHDRFile(file);

                case ".dds":
                    return TexHelper.Instance.LoadFromDDSFile(file, DDS_FLAGS.NONE);

            }

            return null;
        }

        /// <inheritdoc cref="AssetImporter.SupportedExtensions"/>
        public override string[] SupportedExtensions => new[]
        {
            ".bmp", ".jpg", ".png", ".tga", ".hdr", ".dds"
        };
    }
}
