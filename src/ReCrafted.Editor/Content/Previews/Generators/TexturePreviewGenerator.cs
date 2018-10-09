// ReCrafted Editor (c) 2016-2018 Always Too Late

using System.Diagnostics;
using ReCrafted.Editor.Common;
using ReCrafted.Editor.Content.Assets;
using ReCrafted.Editor.Core;

namespace ReCrafted.Editor.Content.Previews.Generators
{
    public class TexturePreviewGenerator : Singleton<TexturePreviewGenerator>, IPreviewGenerator
    {
        public void Initialize()
        {
            Logger.Log("Initialized TexturePreviewGenerator");
        }

        public void Shutdown()
        {
            Logger.Log("Shutdown TexturePreviewGenerator");
        }

        public TextureAsset Generate(Asset asset)
        {
            Debug.Assert(asset is TextureAsset);

            var textureAsset = (TextureAsset)asset;

            Debug.Assert(textureAsset.Mips.Length > 0);

            var mip0 = textureAsset.Mips[0];

            // Generate
            // TODO: Process mip. Scale the texture to `PreviewsCache.DefaultSize` ImageSharp or FreeImage

            // Create asset
            var preview = ContentManager.CreateAsset<TextureAsset>();

            preview.Mips = new TextureAsset.Mip[1];
            preview.Mips[0] = mip0;
            preview.MipCount = 1;
            preview.Format = textureAsset.Format;
            preview.TotalSize = (uint)mip0.Size;
            preview.Initialize();

            // Save to cache

            // Return
            return preview;
        }
    }
}
