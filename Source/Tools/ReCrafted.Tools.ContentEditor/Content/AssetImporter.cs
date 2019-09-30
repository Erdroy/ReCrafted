// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

namespace ReCrafted.Tools.ContentEditor.Content
{
    public abstract class AssetImporter
    {
        public abstract IAssetMetadata LoadMetadata(string sourceFileMeta);
        public abstract IAssetMetadata ShowImportDialog(string sourceFile, string sourceFileMeta, bool forceMetaFile = false);

        public abstract bool ImportAsset(string sourceFile, string targetDirectory, IAssetMetadata metadata);

        /// <summary>
        ///     The list of supported file extensions by this importer.
        /// </summary>
        public abstract string[] SupportedExtensions { get; }
    }
}
