// ReCrafted Editor (c) 2016-2018 Always Too Late

using System.IO;
using ReCrafted.Editor.Content.Assets;

namespace ReCrafted.Editor.Content.Importers
{
    public class TextureImporter : AssetImporterBase
    {
        public override AssetBase ImportAsset(string assetPath)
        {
            var asset = new TextureAsset();

            using (var fs = new FileStream("test.rcasset", FileMode.Create, FileAccess.Write))
            {
                asset.Serialize(fs);
            }

            return asset;
        }

        public override string[] SupportedExtensions => new []{
            "jpg", "png", "tga", "dds"
        };
}
}
