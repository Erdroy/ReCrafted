// ReCrafted Editor (c) 2016-2018 Always Too Late

using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using ReCrafted.Editor.Content.Assets;
using ReCrafted.Editor.Content.Previews.Generators;

namespace ReCrafted.Editor.Content.Previews
{
    public class PreviewsCache : IDisposable
    {
        public const int DefaultSize = 128;

        private readonly Dictionary<Guid, TextureAsset> _cache = new Dictionary<Guid, TextureAsset>();

        private TextureAsset _folderPreview;
        private TextureAsset _defaultAssetPreview;
        private TextureAsset _unknownAssetPreview;

        public void Initialize()
        {
            Instance = this;

            // Initialize generators
            TexturePreviewGenerator.Instance.Initialize();

            _folderPreview = ContentManager.Load<TextureAsset>("Editor/Previews/Folder");
            _defaultAssetPreview = ContentManager.Load<TextureAsset>("Editor/Previews/Default");
            _unknownAssetPreview = ContentManager.Load<TextureAsset>("Editor/Previews/Unknown");
        }

        public void Dispose()
        {
            // Shutdown generators
            TexturePreviewGenerator.Instance.Shutdown();

            _folderPreview.Unload();
            _defaultAssetPreview.Unload();
            _unknownAssetPreview.Unload();
        }

        public TextureAsset GeneratePreview(Asset asset)
        {
            switch (asset)
            {
                case TextureAsset texture:
                    return TexturePreviewGenerator.Instance.Generate(texture);
                default:
                    return _unknownAssetPreview;
            }
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
            // TODO: Read asset header
            // TODO: Load asset if not already loaded
            // TODO: Generate preview
            // TODO: Unload asset if loaded previously
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

            return GeneratePreview(asset);
        }

        public static PreviewsCache Instance { get; private set; }
    }
}
