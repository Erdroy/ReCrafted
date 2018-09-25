// ReCrafted Editor (c) 2016-2018 Always Too Late

using System.IO;
using System.Linq;
using DirectXTexNet;

namespace ReCrafted.Editor.Content.Assets
{
    public class TextureAsset : BinaryAsset
    {
        public class Mip
        {
            public int Size { get; set; }
            public int Width { get; set; }
            public int Height { get; set; }
            public byte[] Data { get; set; }
        }

        protected override void OnSerializeBinary(ushort version, BinaryWriter writer)
        {
            writer.Write((ushort)Format);
            writer.Write((ushort)Width);
            writer.Write((ushort)Height);
            writer.Write((byte)MipCount);

            // Write all mips in reverse order small to large
            foreach(var mip in Mips.Reverse())
            {
                writer.Write((ushort)mip.Size);
                writer.Write(mip.Data, 0, mip.Size);
            }
        }

        protected override void OnDeserializeBinary(ushort version, BinaryReader reader)
        {
        }

        public override void Unload()
        {
            
        }

        public int MipCount { get; set; }
        public int Width { get; set; }
        public int Height { get; set; }
        public DXGI_FORMAT Format { get; set; }

        public Mip[] Mips { get; set; }

        public override AssetType AssetType => AssetType.Texture;
    }
}
