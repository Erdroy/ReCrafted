// ReCrafted Editor (c) 2016-2018 Always Too Late

namespace ReCrafted.Editor.Content
{
    public abstract class AssetImporterBase<TImporter, TImporterSettings> : IAssetImporter
        where TImporter : new()
        where TImporterSettings : IImportSettings
    {
        public abstract Asset ImportAsset(string inputFile, string outputFile, TImporterSettings settings);
        
        public abstract AssetType SupportedAssetType { get; }
        public abstract string[] SupportedExtensions { get; }

        private static TImporter _instance;
        public static TImporter Instance
        {
            get
            {
                if (_instance == null)
                    _instance = new TImporter();

                return _instance;
            }
        }
    }

    public interface IAssetImporter
    {
        AssetType SupportedAssetType { get; }
        string[] SupportedExtensions { get; }
    }
}
