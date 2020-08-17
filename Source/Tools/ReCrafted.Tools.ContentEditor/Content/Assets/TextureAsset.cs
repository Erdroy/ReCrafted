// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

using System.Diagnostics;
using System.Drawing;
using System.IO;
using DirectXTexNet;

namespace ReCrafted.Tools.ContentEditor.Content.Assets
{
    public class TextureAsset : BinaryAsset
    {
        public class Mip
        {
            public int Size { get; set; }
            public int Width { get; set; }
            public int Height { get; set; }
            public int Pitch { get; set; }
            public byte[] Data { get; set; }
        }

        public override void Initialize()
        {
            Debug.Assert(MipCount > 0);
            Debug.Assert(Mips != null);
        }

        public override void Unload()
        {
            Mips = null;
        }

        /// <summary>
        ///     Gets pixel at given coordinates.
        /// </summary>
        /// <param name="x">The x coordinate.</param>
        /// <param name="y">The y coordinate.</param>
        /// <param name="mipLevel">The mip level.</param>
        /// <returns>The pixel at given coordinate.</returns>
        public Color GetPixel(int x, int y, int mipLevel = 0)
        {
            Debug.Assert(x >= 0 && x < Width);
            Debug.Assert(y >= 0 && y < Height);
            Debug.Assert(mipLevel >= 0 && mipLevel < MipCount);
            Debug.Assert(MipCount > 0);
            Debug.Assert(Mips != null);
            Debug.Assert(Format == DXGI_FORMAT.R8G8B8A8_UNORM);

            var mip = Mips[mipLevel];
            var index = x * 4 + y * mip.Pitch;

            return Color.FromArgb(mip.Data[index + 3], mip.Data[index + 0], mip.Data[index + 1], mip.Data[index + 2]);
        }

        protected override void OnSerializeBinary(ushort version, BinaryWriter writer)
        {
            Debug.Assert(MipCount > 0);

            // Write texture header
            writer.Write(TotalSize);
            writer.Write((ushort) Format);
            writer.Write((byte) MipCount);

            // Write mips
            foreach (var mip in Mips)
            {
                Debug.Assert(mip.Width > 0);
                Debug.Assert(mip.Height > 0);
                Debug.Assert(mip.Pitch > 0);
                Debug.Assert(mip.Size > 0);

                writer.Write((ushort) mip.Width);
                writer.Write((ushort) mip.Height);
                writer.Write((ushort) mip.Pitch);
                writer.Write(mip.Size);
                writer.Write(mip.Data, 0, mip.Size);
            }
        }

        protected override void OnDeserializeBinary(ushort version, BinaryReader reader)
        {
            TotalSize = reader.ReadUInt32();
            Format = (DXGI_FORMAT) reader.ReadUInt16();
            MipCount = reader.ReadByte();

            Mips = new Mip[MipCount];

            for (var i = 0; i < MipCount; i++)
            {
                var width = reader.ReadUInt16();
                var height = reader.ReadUInt16();
                var pitch = reader.ReadUInt16();
                var size = reader.ReadInt32();

                Debug.Assert(width > 0);
                Debug.Assert(height > 0);
                Debug.Assert(pitch > 0);
                Debug.Assert(size > 0);

                // Create mip
                var mip = Mips[i] = new Mip
                {
                    Width = width,
                    Height = height,
                    Pitch = pitch,
                    Size = size
                };

                // Read mip
                mip.Data = reader.ReadBytes(size);
            }
        }

        public void Convert(DXGI_FORMAT format)
        {
            if (format == DXGI_FORMAT.R8_UINT)
            {
                foreach (var mip in Mips)
                {
                    var mipSize = mip.Width * mip.Height * 1;
                    var newData = new byte[mipSize];

                    for (var i = 0; i < mip.Data.Length; i += 4)
                    {
                        newData[i / 4] = mip.Data[i + 0]; // R
                        // G
                        // B
                        // A
                    }

                    mip.Data = newData;
                    mip.Pitch = mip.Width * 1;
                }

                Format = format;
            }
        }

        public Mip[] Mips { get; set; }
        public DXGI_FORMAT Format { get; set; }
        public int MipCount { get; set; }
        public uint TotalSize { get; set; }

        public int Width => Mips[0].Width;
        public int Height => Mips[0].Height;
        public override AssetType AssetType => AssetType.Texture2D;
    }
}