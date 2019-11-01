// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using System;
using System.Collections;
using System.Collections.Concurrent;
using System.Drawing;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Threading;
using System.Windows.Forms;
using DirectXTexNet;
using ReCrafted.Tools.ContentEditor.Content.Assets;
using ReCrafted.Tools.ContentEditor.Properties;
using Image = System.Drawing.Image;

namespace ReCrafted.Tools.ContentEditor.Content
{
    public class PreviewIconManager : IDisposable
    {
        private delegate void RefreshContentView(string imageKey, Image image, GenerateQueueItem item);

        private class GenerateQueueItem
        {
            public string ImageKey { get; set; }
            public AssetCache.AssetCacheItem Asset { get; set; }
            public string FileName { get; set; }
            public Action OnDone { get; set; }
        }

        private readonly ConcurrentQueue<GenerateQueueItem> _generateQueue = new ConcurrentQueue<GenerateQueueItem>();

        private RefreshContentView _refreshContentView;
        private bool _isRunning;
        private Thread _thread;

        public void Run()
        {
            _isRunning = true;

            _refreshContentView = ((imageKey, image, item) =>
            {
                ContentViewImages.Images.Add(imageKey, image);
                item.OnDone();
            });
            _thread = new Thread(Worker);
            _thread.Start();

            ContentViewImages = new ImageList
            {
                ImageSize = new Size(64, 64),
                ColorDepth = ColorDepth.Depth32Bit
            };

            var resourceSet = Resources.ResourceManager.GetResourceSet(CultureInfo.InvariantCulture, true, false);
            if (resourceSet != null)
            {
                foreach (DictionaryEntry entry in resourceSet)
                {
                    if (entry.Value is Bitmap value)
                    {
                        ContentViewImages.Images.Add((string)entry.Key, value);
                    }
                }
            }

            Instance = this;
        }

        public void Dispose()
        {
            _isRunning = false;
            _thread.Join();
        }

        private void Worker()
        {
            while (_isRunning)
            {
                if (!_generateQueue.TryDequeue(out var item))
                {
                    Thread.Sleep(100);
                    continue;
                }

                var fileName = $"./Cache/{item.Asset.AssetGuid}.jpg";
                var imageKey = item.ImageKey;

                Image image = null;
                if (File.Exists(fileName))
                {
                     if (!ContentViewImages.Images.ContainsKey(imageKey))
                     {
                         image = Image.FromFile(fileName);
                     }
                }
                else
                {
                    // Load texture asset
                    var texture = new TextureAsset();
                    using (var fs = new FileStream(item.FileName, FileMode.Open, FileAccess.Read))
                    {
                        texture.Deserialize(fs);
                    }

                    var previewImage = TexHelper.Instance.Initialize(
                        new TexMetadata(
                            texture.Width, 
                            texture.Height,
                            1, 1, 1, 0, 0, 
                            texture.Format, 
                            TEX_DIMENSION.TEXTURE2D), 
                        CP_FLAGS.BPP24);


                    var mip0 = texture.Mips.First();
                    Marshal.Copy(mip0.Data, 0, previewImage.GetImage(0).Pixels, mip0.Size);

                    ScratchImage scaledPreviewImage;
                    if (previewImage.GetMetadata().Format == DXGI_FORMAT.BC7_UNORM)
                    {
                        // Decompress image now
                        var decompressedPreviewImage = previewImage.Decompress(DXGI_FORMAT.R8G8B8A8_UNORM);
                        scaledPreviewImage = decompressedPreviewImage.Resize(64, 64, TEX_FILTER_FLAGS.FANT);
                        decompressedPreviewImage.Dispose();
                    }
                    else
                    {
                        scaledPreviewImage = previewImage.Resize(64, 64, TEX_FILTER_FLAGS.FANT);
                    }

                    if (!Directory.Exists("Cache"))
                        Directory.CreateDirectory("Cache");

                    scaledPreviewImage.SaveToWICFile(0, WIC_FLAGS.NONE, 
                        new Guid(0x19e4a5aa, 0x5662, 0x4fc5, 0xa0, 0xc0, 0x17, 0x58, 0x2, 0x8e, 0x10, 0x57), 
                        $"./Cache/{item.Asset.AssetGuid}.jpg");

                    scaledPreviewImage.Dispose();
                    previewImage.Dispose();
                }

                if (image != null)
                {
                    MainForm.Instance.Invoke(_refreshContentView, imageKey, image, item);
                }
            }
        }

        public string GenerateOrLoad(string file, Action onDone, bool isRelativeToContent = false)
        {
            var contentPath = Path.Combine(Settings.Current.GameDirectory, "Content");

            // Get relative path to ./Content
            var relativeFileName = isRelativeToContent ? file : file.Remove(0, contentPath.Length).Remove(0, 1);
            var asset = AssetCache.Assets.FirstOrDefault(x => relativeFileName.Equals(x.AssetFile));

            // Select asset preview based on asset type
            if (asset != null && asset.AssetType == AssetType.Texture2D)
            {
                var imageKey = asset.AssetGuid.ToString();

                _generateQueue.Enqueue(new GenerateQueueItem
                {
                    ImageKey = imageKey,
                    Asset = asset,
                    FileName = file,
                    OnDone = onDone
                });

                return imageKey;
            }

            return "Icon_Unknown";
        }

        /// <summary>
        ///     The list of all images used by the ContentView.
        /// </summary>
        public ImageList ContentViewImages { get; private set; }

        public static PreviewIconManager Instance { get; private set; }
    }
}
