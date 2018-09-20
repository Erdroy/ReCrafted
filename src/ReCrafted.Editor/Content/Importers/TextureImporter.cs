// ReCrafted Editor (c) 2016-2018 Always Too Late

using ReCrafted.Editor.Content.Assets;

namespace ReCrafted.Editor.Content.Importers
{
    public class TextureImporter : AssetImporterBase
    {
        public override AssetBase ImportAsset(string assetPath)
        {
            var asset1 = new TextureAsset();
            asset1.SerializeToFile("test.rcasset");

            var asset2 = new TextureAsset();
            asset2.DeserializeFromFile("test.rcasset");

            return asset2;
        }

        public override string[] SupportedExtensions => new []{
            "jpg", "png", "tga", "dds"
        };
}
}
