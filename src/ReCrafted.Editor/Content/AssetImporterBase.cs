// ReCrafted Editor (c) 2016-2018 Always Too Late

namespace ReCrafted.Editor.Content
{
    public abstract class AssetImporterBase
    {
        public abstract AssetBase ImportAsset(string inputFile, string outputFile);

        public abstract AssetType SupportedAssetType { get; }
        public abstract string[] SupportedExtensions { get; }
    }
}
