// ReCrafted Editor (c) 2016-2018 Always Too Late

namespace ReCrafted.Editor.Content
{
    public abstract class AssetImporterBase<T> where T : IImportSettings
    {
        public abstract Asset ImportAsset(string inputFile, string outputFile, T settings);

        public abstract AssetType SupportedAssetType { get; }
        public abstract string[] SupportedExtensions { get; }
    }
}
