// ReCrafted Editor (c) 2016-2018 Always Too Late

using System;
using System.Collections.Generic;
using System.IO;

namespace ReCrafted.Editor.Content
{
    public class ContentManager : IDisposable
    {
        private readonly Dictionary<Guid, Asset> _assetMap = new Dictionary<Guid, Asset>();

        private const string ContentDirectory = "../content";
        private string _contentDir;
        
        public ContentManager()
        {
            Instance = this;
        }

        public void Initialize()
        {
            _contentDir = Path.Combine(Environment.CurrentDirectory, ContentDirectory);
            if(!Directory.Exists(_contentDir))
                throw new Exception("Game content directory is missing!");


        }

        public void Update()
        {
        }

        public void Dispose()
        {
        }

        private void RegisterAsset(Asset asset)
        {
            lock (_assetMap)
            {
                _assetMap.Add(asset.AssetGuid, asset);
            }
        }

        /// <summary>
        /// Creates empty virtual asset of given type.
        /// </summary>
        /// <typeparam name="TAsset">The target asset class type.</typeparam>
        /// <param name="register">Should this asset be registered with it's current GUID?</param>
        /// <returns>The created asset.</returns>
        public static TAsset CreateAsset<TAsset>(bool register = true) where TAsset : Asset, new()
        {
            var asset = new TAsset
            {
                AssetGuid = Guid.NewGuid()
            };

            if (register)
            {
                Instance.RegisterAsset(asset);
            }
            
            return asset;
        }

        /// <summary>
        /// Looks for asset with given guid.
        /// </summary>
        /// <typeparam name="TAsset">The target asset class type.</typeparam>
        /// <param name="guid">The asset guid.</param>
        /// <returns>The found asset or null when not found.</returns>
        public static TAsset FindAsset<TAsset>(Guid guid) where TAsset : Asset
        {
            if (!Instance._assetMap.ContainsKey(guid))
                return null;
            return (TAsset)Instance._assetMap[guid];
        }

        /// <summary>
        /// Loads given asset file.
        /// </summary>
        /// <typeparam name="TAsset">The target asset type.</typeparam>
        /// <param name="assetName">The asset name. Relative to '../content/, without extension.</param>
        /// <returns>The loaded asset.</returns>
        public static TAsset Load<TAsset>(string assetName) where TAsset : Asset, new()
        {
            // Build asset file path
            var assetFile = Path.Combine(ContentDirectory, assetName);
            if (!assetName.EndsWith(".rcasset"))
                assetFile += ".rcasset";
            
            // Find asset with given name
            if (!File.Exists(assetFile))
                throw new Exception($"Could not find file for asset '{assetName}' ({assetFile}).");

            using (var fs = new FileStream(assetFile, FileMode.Open, FileAccess.Read))
            {
                // Create asset
                var asset = CreateAsset<TAsset>(false);

                // Deserialize asset
                asset.Deserialize(fs);

                // Register asset
                Instance.RegisterAsset(asset);

                // Initialize asset and return
                asset.Initialize();
                return asset;
            }
        }
        
        public static ContentManager Instance { get; private set; }
    }
}
