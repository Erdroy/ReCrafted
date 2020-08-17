// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

using System;

namespace ReCrafted.Tools.ContentEditor.Content
{
    public abstract class AssetImporter
    {
        public abstract IAssetMetadata LoadMetadata(string sourceFileMeta);
        public abstract IAssetMetadata ShowImportDialog(string sourceFile, string sourceFileMeta, string targetDirectory, bool forceMetaFile = false);

        public abstract bool ImportAsset(string sourceFile, string targetDirectory, IAssetMetadata metadata, Action<float, string> importProgress = null);

        /// <summary>
        ///     The list of supported file extensions by this importer.
        /// </summary>
        public abstract string[] SupportedExtensions { get; }
    }
}
