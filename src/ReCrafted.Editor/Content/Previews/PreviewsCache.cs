// ReCrafted Editor (c) 2016-2018 Always Too Late

using System;
using System.Collections.Generic;
using ReCrafted.Editor.Content.Assets;

namespace ReCrafted.Editor.Content.Previews
{
    public class PreviewsCache : IDisposable
    {
        private readonly Dictionary<Guid, TextureAsset> _cache = new Dictionary<Guid, TextureAsset>();

        private TextureAsset _folderPreview;
        private TextureAsset _defaultAssetPreview;
        private TextureAsset _unknownAssetPreview;

        public void Initialize()
        {
            Instance = this;

            _folderPreview = ContentManager.Load<TextureAsset>("Editor/Previews/Folder");
            _defaultAssetPreview = ContentManager.Load<TextureAsset>("Editor/Previews/Default");
            _unknownAssetPreview = ContentManager.Load<TextureAsset>("Editor/Previews/Unknown");
        }

        public void Dispose()
        {
            _defaultAssetPreview.Unload();
            _unknownAssetPreview.Unload();
        }

        public static void GeneratePreview(Asset asset, Action<TextureAsset> onFinish)
        {

        }

        public TextureAsset GetFolderPreview()
        {
            return _folderPreview;
        }

        public TextureAsset GetDefaultPreview()
        {
            return _defaultAssetPreview;
        }

        public TextureAsset GetPreview(string fileName)
        {
            return _unknownAssetPreview;
        }

        public TextureAsset GetPreview(Asset asset)
        {
            if (asset == null)
                return _unknownAssetPreview;

            if (_cache.ContainsKey(asset.AssetGuid))
            {
                return _cache[asset.AssetGuid];
            }

            return _defaultAssetPreview;
        }

        public static PreviewsCache Instance { get; private set; }
    }
}
