// ReCrafted Editor (c) 2016-2018 Always Too Late

namespace ReCrafted.Editor.Content.Importers
{
    public class TextureImporter : AssetImporterBase
    {
        public override AssetBase ImportAsset(string assetPath)
        {
            return null;
        }

        public override string[] SupportedExtensions => new []{
            "jpg", "png", "tga", "dds"
        };
}
}
