// ReCrafted Editor (c) 2016-2018 Always Too Late

using System.IO;
using DirectXTexNet;
using ReCrafted.Editor.Content.Assets;

namespace ReCrafted.Editor.Content.Importers
{
    public class TextureImporter : AssetImporterBase
    {
        public override AssetBase ImportAsset(string inputFile, string outputFile)
        {
            var image = LoadImage(inputFile);

            var images = image.GetImageCount();

            var asset = new TextureAsset
            {
                
            };


            // Serialize asset
            using (var fs = new FileStream("test.rcasset", FileMode.Create, FileAccess.Write))
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
            "jpg", "png", "tga", "dds"
        };
}
}
