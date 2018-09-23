// ReCrafted Editor (c) 2016-2018 Always Too Late

using System;
using System.IO;
using DirectXTexNet;
using ReCrafted.Editor.Content.Assets;
using ReCrafted.Editor.Utilities;

namespace ReCrafted.Editor.Content.Importers
{
    public class TextureImporter : AssetImporterBase<TextureImporter.Settings>
    {
        public class Settings : IImportSettings
        {
            public bool GenerateMipMaps { get; set; } = true;
        }

        public override AssetBase ImportAsset(string inputFile, string outputFile, Settings settings)
        {
            var image = LoadImage(inputFile);
            var baseImage = image.GetImage(0);
            var mipCount = image.GetImageCount();
            
            var asset = new TextureAsset
            {
                Format = baseImage.Format,
                Width = baseImage.Width,
                Height = baseImage.Height
            };

            if (mipCount == 1 && settings.GenerateMipMaps)
            {
                if (!MathUtils.IsPowerOf2(baseImage.Width) || !MathUtils.IsPowerOf2(baseImage.Height))
                    throw new Exception($"Cannot generate mip maps for texture '{inputFile}', because it's width or height is not within power of 2!");

                //  Generate mip maps
                var mipMaps = image.GenerateMipMaps(0, 0);

                // Set image 
                image.Dispose();
                image = mipMaps;

                mipCount = mipMaps.GetImageCount();
            }

            // Serialize asset
            using (var fs = new FileStream(outputFile, FileMode.Create, FileAccess.Write))
            {
                asset.Serialize(fs);
            }

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
