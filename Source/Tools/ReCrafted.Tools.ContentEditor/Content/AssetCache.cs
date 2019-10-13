// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

namespace ReCrafted.Tools.ContentEditor.Content
{
    public static class AssetCache
    {
        public class AssetCacheItem
        {
            public string AssetFile { get; set; }
            public Guid AssetGuid { get; set; }
            public AssetType AssetType { get; set; }
        }

        private static List<AssetCacheItem> _cache = new List<AssetCacheItem>();

        public static void LoadCache(bool rebuildIfNotFound = false)
        {
            var loaded = false;
            lock (_cache)
            { 
                // Create file name
                var outputFile = Path.Combine(Settings.Current.GameDirectory, "Content", "Assets.rccache");

                if (File.Exists(outputFile))
                {
                    // Load from Content/Assets.rccache file
                    using (var fs = new FileStream(outputFile, FileMode.Open, FileAccess.Read))
                    {
                        using (var stream = new BinaryReader(fs))
                        {
                            var numItems = stream.ReadInt32();

                            // Clear cache
                            _cache.Clear();
                            _cache = new List<AssetCacheItem>(numItems);

                            for (var i = 0; i < numItems; i++)
                            {
                                var assetFile = stream.ReadString();
                                var assetGuid = new Guid(stream.ReadBytes(16));
                                var assetType = (AssetType) stream.ReadByte();

                                if (!File.Exists(Path.Combine(Settings.Current.GameDirectory, "Content", assetFile)))
                                    continue;

                                _cache.Add(new AssetCacheItem
                                {
                                    AssetType = assetType,
                                    AssetGuid = assetGuid,
                                    AssetFile = assetFile
                                });
                            }
                        }
                    }

                    loaded = true;
                }
            }

            if (!loaded && rebuildIfNotFound)
            {
                RebuildCache();
            }

            // Save
            SaveCache();
        }

        public static void SaveCache()
        {
            lock (_cache)
            {
                // Create file name
                var outputFile = Path.Combine(Settings.Current.GameDirectory, "Content", "Assets.rccache");

                // Save to Content/Assets.rccache file
                using (var fs = new FileStream(outputFile, FileMode.Create, FileAccess.Write))
                {
                    using (var stream = new BinaryWriter(fs))
                    {
                        stream.Write(_cache.Count);

                        foreach (var cache in _cache)
                        {
                            stream.Write(cache.AssetFile);
                            stream.Write(cache.AssetGuid.ToByteArray());
                            stream.Write((byte)cache.AssetType);
                        }
                    }
                }
            }
        }

        public static void AddAssetType(string rawAssetFile, Guid assetGuid, AssetType assetType)
        {
            var assetFile = rawAssetFile.Replace(Path.Combine(Settings.Current.GameDirectory, "Content") + "\\", "");

            lock (_cache)
            {
                // Remove duplicates
                _cache.RemoveAll(x => x.AssetFile == assetFile || x.AssetGuid == assetGuid);

                // Add item
                _cache.Add(new AssetCacheItem
                {
                    AssetType = assetType,
                    AssetGuid = assetGuid,
                    AssetFile = assetFile
                });

                // Save
                SaveCache();
            }
        }

        public static void RemoveAssetType(string assetFile)
        {
            lock (_cache)
            {
                _cache.RemoveAll(x => x.AssetFile.EndsWith(assetFile));

                // Save
                SaveCache();
            }
        }

        public static void RemoveAssetType(Guid assetGuid)
        {
            lock (_cache)
            {
                _cache.RemoveAll(x => x.AssetGuid == assetGuid);

                // Save
                SaveCache();
            }
        }

        public static void RebuildCache()
        {
            // TODO: Show progress

            lock (_cache)
            {
                var contentDirectory = Path.Combine(Settings.Current.GameDirectory, "Content");

                // Get all asset files inside Content directory
                var assetFiles = Directory.GetFiles(contentDirectory, "*.rcasset", SearchOption.AllDirectories);

                // Clear cache
                _cache.Clear();
                _cache = new List<AssetCacheItem>(assetFiles.Length);

                foreach (var fileName in assetFiles)
                {
                    using (var fs = new FileStream(fileName, FileMode.Open, FileAccess.Read))
                    {
                        using (var stream = new BinaryReader(fs))
                        {
                            var assetFile = fileName.Replace(Path.Combine(Settings.Current.GameDirectory, "Content") + "\\", "");

                            // TODO: Use Asset class somehow

                            var header = stream.ReadBytes(3);

                            if (header.SequenceEqual(Asset.AssetMagic))
                            {
                                // This is binary asset

                                // Read asset header 
                                var assetVersion = stream.ReadUInt16();
                                var assetType = (AssetType)stream.ReadByte();
                                var assetGuid = new Guid(stream.ReadBytes(Guid.Empty.ToByteArray().Length));

                                _cache.Add(new AssetCacheItem
                                {
                                    AssetType = assetType,
                                    AssetGuid = assetGuid,
                                    AssetFile = assetFile
                                });

                                // Ok, we're done here.
                            }
                            else
                            {
                                // This is (probably) JSON asset
                                fs.Position = 0;

                                using (var sr = new StreamReader(fs))
                                {
                                    using (var jr = new JsonTextReader(sr))
                                    {
                                        var jsonObject = JObject.Load(jr);

                                        if (jsonObject.ContainsKey("AssetType") && jsonObject.ContainsKey("AssetGuid"))
                                        {
                                            var assetGuid = Guid.Parse(jsonObject["AssetGuid"].ToObject<string>());
                                            var assetType = (AssetType)jsonObject["AssetType"].ToObject<byte>();

                                            _cache.Add(new AssetCacheItem
                                            {
                                                AssetType = assetType,
                                                AssetGuid = assetGuid,
                                                AssetFile = assetFile
                                            });

                                            // Ok, we're done here.
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        /// <summary>
        ///     The cached assets list.
        /// </summary>
        public static IReadOnlyList<AssetCacheItem> Assets
        {
            get
            {
                lock (_cache)
                {
                    var assetCacheItems = _cache.ToArray();
                    return assetCacheItems;
                }
            }
        }
    }
}
