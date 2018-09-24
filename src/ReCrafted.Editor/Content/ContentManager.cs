// ReCrafted Editor (c) 2016-2018 Always Too Late

using System;
using System.IO;

namespace ReCrafted.Editor.Content
{
    public class ContentManager : IDisposable
    {
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
                var asset = new TAsset();
                asset.Deserialize(fs);
                asset.Initialize();
                return asset;
            }
        }
        
        public static ContentManager Instance { get; private set; }
    }
}
