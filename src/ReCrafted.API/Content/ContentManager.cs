// ReCrafted (c) 2016-2018 Always Too Late

using System.Diagnostics;

namespace ReCrafted.API.Content
{
    public delegate void AssetLoadCallback(Asset asset);

    public static class ContentManager
    {
        public static TAsset LoadAsset<TAsset>(string assetFile) where TAsset : Asset
        {
            Debug.Assert(!string.IsNullOrEmpty(assetFile));

            // Create asset object
            var assetObject = Object.New<TAsset>();

            // Load asset
            return (TAsset)ContentManagerInternals.LoadAsset(assetObject.NativePtr, assetFile);
        }

        public static void LoadAsset<TAsset>(string assetFile, AssetLoadCallback onLoad) where TAsset : Asset
        {
            Debug.Assert(!string.IsNullOrEmpty(assetFile));

            // Create asset object
            var assetObject = Object.New<TAsset>();

            // Load asset async
            ContentManagerInternals.LoadAssetAsync(assetObject.NativePtr, assetFile, onLoad);
        }

        public static void UnloadAsset<TAsset>(TAsset asset) where TAsset : Asset
        {
            Debug.Assert(asset);
            ContentManagerInternals.UnloadAsset(asset.NativePtr);
        }
    }
}
