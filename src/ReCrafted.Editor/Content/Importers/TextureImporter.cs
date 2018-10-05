// ReCrafted Editor (c) 2016-2018 Always Too Late

using System;
using System.IO;
using System.Runtime.InteropServices;
using DirectXTexNet;
using ReCrafted.Editor.Content.Assets;
using ReCrafted.Editor.Utilities;

namespace ReCrafted.Editor.Content.Importers
{
    public class TextureImporter : AssetImporterBase<TextureImporter, TextureImporter.Settings>
    {
        public class Settings : IImportSettings
        {
            public TEX_COMPRESS_FLAGS CompressionFlags = TEX_COMPRESS_FLAGS.BC7_QUICK;
            public DXGI_FORMAT CompressionFormat = DXGI_FORMAT.BC7_UNORM;
            public bool Compress = false;
            public bool GenerateMipMaps = true;
        }

        public override Asset ImportAsset(string inputFile, string outputFile, Settings settings)
        {
            var image = LoadImage(inputFile);

            if (settings.Compress)
            {
                // Threshold is 0.5, because it is being used only by BC1. Maybe expose if needed
                var compressedImage = image.Compress(settings.CompressionFormat, settings.CompressionFlags, 0.5f);
                
                // Set image
                image.Dispose();
                image = compressedImage;
            }

            var baseImage = image.GetImage(0);
            var mipCount = image.GetImageCount();

            if (mipCount == 1 && settings.GenerateMipMaps)
            {
                if (!MathUtils.IsPowerOf2(baseImage.Width) || !MathUtils.IsPowerOf2(baseImage.Height))
                    throw new Exception($"Cannot generate mip maps for texture '{inputFile}', because it's width or height is not within power of 2!");

                // Generate mip maps
                var mipMaps = image.GenerateMipMaps(0, 0);

                // Set image 
                image.Dispose();
                image = mipMaps;

                mipCount = mipMaps.GetImageCount();
            }

            // Create asset
            var asset = ContentManager.CreateAsset<TextureAsset>();
            asset.Format = baseImage.Format;
            asset.MipCount = mipCount;
            asset.Mips = new TextureAsset.Mip[mipCount];

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

            // Serialize asset
            using (var fs = new FileStream(outputFile, FileMode.Create, FileAccess.Write))
            {
                asset.Serialize(fs);
            }

            // TODO: Generate preview icon

            // Dispose image
            image.Dispose();
            return asset;
        }

        private static ScratchImage LoadImage(string file)
        {
            var extension = Path.GetExtension(file)?.ToLower();
            
            switch (extension)
            {
                case ".bmp":
                case ".jpg":
                case ".png":
                    return TexHelper.Instance.LoadFromWICFile(file, WIC_FLAGS.NONE);

                case ".tga":
                    return TexHelper.Instance.LoadFromTGAFile(file);

                case ".hdr":
                    return TexHelper.Instance.LoadFromHDRFile(file);

                case ".dds":
                    return TexHelper.Instance.LoadFromDDSFile(file, DDS_FLAGS.NONE);

            }

            return null;
        }

        public override AssetType SupportedAssetType => AssetType.Texture;

        public override string[] SupportedExtensions => new []{
            "bmp", "jpg", "png", "tga", "hdr", "dds"
        };
}
}
