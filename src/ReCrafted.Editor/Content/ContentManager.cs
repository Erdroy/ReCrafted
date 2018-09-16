// ReCrafted Editor (c) 2016-2018 Always Too Late

using System;
using System.IO;
using ReCrafted.Editor.Content.Importers;

namespace ReCrafted.Editor.Content
{
    public class ContentManager : IDisposable
    {
        private const string AssetsDirectory = "../../game/content";

        private string _contentDir;

        public ContentManager()
        {
            Instance = this;
        }

        public void Initialize()
        {
            _contentDir = Path.Combine(Environment.CurrentDirectory, AssetsDirectory);
            if(!Directory.Exists(_contentDir))
                throw new Exception("Game content directory is missing!");
        }

        public void Update()
        {

        }

        public void Dispose()
        {
        }

        public void MountAssetDirectory(string assetDirectory)
        {

        }
        
        public void MountAssets()
        {

        }

        public static void Load(string assetName)
        {
            // Find asset with given name

        }

        public AssetImporterBase[] Importers = {
            new TextureImporter()
        };

        public static ContentManager Instance { get; private set; }
    }
}
