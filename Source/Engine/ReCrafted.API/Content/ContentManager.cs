// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using System;
using System.Diagnostics;

namespace ReCrafted.API.Content
{
    /// <summary>
    ///     ContentManager class. Provides asset management functionality (loading etc.).
    /// </summary>
    public partial class ContentManager
    {
        /// <summary>
        ///     Creates and returns virtual asset instance of given <see cref="TAsset"/> type.
        /// </summary>
        /// <typeparam name="TAsset">The designated asset type.</typeparam>
        /// <returns>The created virtual asset instance.</returns>
        /// <remarks>Can be called only from the MainThread.</remarks>
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
        /// <typeparam name="TAsset">The asset type. The type must be derived from <see cref="Asset"/>.</typeparam>
        /// <param name="assetFile">The asset file, relative to '../Content/', file extension is not needed.</param>
        /// <returns>The loaded asset or nullptr when failed.</returns>
        /// <remarks>Can be called only from the MainThread.</remarks>
        public static TAsset LoadAsset<TAsset>(string assetFile) where TAsset : Asset
        {
            Debug.Assert(!string.IsNullOrEmpty(assetFile));

            // Try to find asset
            var assetObject = (TAsset)InternalInternalFindAsset(assetFile);

            if (assetObject)
            {
                // Asset already loaded, so just add reference.
                assetObject.AddRef();
                return assetObject;
            }

            // Create asset object
            assetObject = Object.New<TAsset>();

            // Load asset
            InternalInternalLoadAssetSync(assetObject.NativePtr, assetFile);
            return assetObject;
        }

        /// <summary>
        ///     Loads given asset file asynchronously with given target asset type.
        /// </summary>
        /// <typeparam name="TAsset">The asset type. The type must be derived from <see cref="Asset"/>.</typeparam>
        /// <param name="assetFile">The asset file, relative to '../Content/', file extension is not needed.</param>
        /// <param name="onLoad">The asset load callback. Called from main thread when asset has been fully loaded.</param>
        /// <returns>The loaded asset or nullptr when failed.</returns>
        /// <remarks>Can be called only from the MainThread.</remarks>
        public static Asset LoadAsset<TAsset>(string assetFile, Action<Asset> onLoad) where TAsset : Asset
        {
            Debug.Assert(!string.IsNullOrEmpty(assetFile));

            // Try to find asset
            var assetObject = (TAsset)InternalInternalFindAsset(assetFile);

            if (assetObject)
            {
                // Asset already loaded, so just add reference.
                assetObject.AddRef();

                if (assetObject.IsLoading)
                {
                    // Add asset load event
                    InternalInternalAddAssetLoadEvent(assetObject.NativePtr, onLoad);
                }
                else
                {
                    // Invoke load callback
                    onLoad(assetObject);
                }

                return assetObject;
            }

            // Create asset object
            assetObject = Object.New<TAsset>();

            // Load asset async
            InternalInternalLoadAssetAsync(assetObject.NativePtr, assetFile, onLoad);
            return assetObject;
        }
    }
}
