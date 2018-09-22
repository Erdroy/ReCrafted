// ReCrafted Editor (c) 2016-2018 Always Too Late

using System.IO;
using DirectXTexNet;

namespace ReCrafted.Editor.Content.Assets
{
    public class TextureAsset : AssetBase
    {
        protected override void OnSerialize(ushort version, BinaryWriter writer)
        {
        }

        protected override void OnDeserialize(ushort version, BinaryReader reader)
        {
        }

        public override void Unload()
        {
            
        }

        public int Width { get; set; }
        public int Height { get; set; }
        public int Length { get; set; }

        public DXGI_FORMAT Format { get; set; }
        
        public override AssetType AssetType => AssetType.Texture;
    }
}
