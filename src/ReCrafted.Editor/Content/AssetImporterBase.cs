// ReCrafted Editor (c) 2016-2018 Always Too Late

namespace ReCrafted.Editor.Content
{
    public abstract class AssetImporterBase
    {
        public abstract AssetBase ImportAsset(string assetPath);

        public abstract string[] SupportedExtensions { get; }
    }
}
