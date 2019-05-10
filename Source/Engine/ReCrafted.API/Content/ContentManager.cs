// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using System;
using System.Diagnostics;

namespace ReCrafted.API.Content
{
    /// <summary>
    ///     Asset load callback.
    /// </summary>
    /// <param name="asset">The loaded asset.</param>
    public delegate void AssetLoadCallback(Asset asset);

    /// <summary>
    ///     ContentManager class. Provides asset management functionality (loading etc.).
    /// </summary>
    public partial class ContentManager
    {
        public static TAsset CreateVirtualAsset<TAsset>() where TAsset : Asset
        {
            // Create asset object
            var assetObject = Object.New<TAsset>();

            // Create virtual asset
            InternalInternalInitVirtualAsset(assetObject.NativePtr);

            return assetObject;
        }

        /// <summary>
        ///     Loads given asset file with given target asset type.
        /// </summary>
        /// <typeparam name="TAsset">The asset type.</typeparam>
        /// <param name="assetFile">The asset file, relative to '../Content/', file extension is not needed.</param>
        /// <returns>The created file or nullptr when failed.</returns>
        public static TAsset LoadAsset<TAsset>(string assetFile) where TAsset : Asset
        {
            Debug.Assert(!string.IsNullOrEmpty(assetFile));

            // Create asset object
            var assetObject = Object.New<TAsset>();

            // Load asset
            InternalInternalLoadAssetSync(assetObject.NativePtr, assetFile);

            return assetObject;
        }

        public static void LoadAsset<TAsset>(string assetFile, AssetLoadCallback onLoad) where TAsset : Asset
        {
            Debug.Assert(!string.IsNullOrEmpty(assetFile));

            // Create asset object
            var assetObject = Object.New<TAsset>();

            // Load asset async
            //ContentManagerInternals.LoadAssetAsync(assetObject.NativePtr, assetFile, onLoad);

            throw new NotImplementedException();
        }
    }
}
