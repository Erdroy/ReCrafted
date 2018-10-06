// ReCrafted Editor (c) 2016-2018 Always Too Late

using System;
using System.Diagnostics;
using System.IO;
using System.Numerics;
using DirectXTexNet;
using ReCrafted.Editor.Core;
using Veldrid;

namespace ReCrafted.Editor.Content.Assets
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

            var gd = EditorApplication.Current.GraphicsDevice;
            Texture = gd.ResourceFactory.CreateTexture(new TextureDescription
            {
                Width = (uint)Width,
                Height = (uint)Height,
                ArrayLayers = 1,
                Depth = 1,
                MipLevels = (uint)MipCount,
                Type = TextureType.Texture2D,
                Format = PixelFormat,
                SampleCount = TextureSampleCount.Count1,
                Usage = TextureUsage.Sampled
            });

            for (var i = 0u; i < MipCount; i++)
            {
                gd.UpdateTexture(Texture, Mips[i].Data, 0, 0, 0, (uint)Width, (uint)Height, 0, i, 0);
            }

            TextureView = gd.ResourceFactory.CreateTextureView(Texture);
        }

        public override void Unload()
        {
            Texture.Dispose();
            TextureView.Dispose();
            Mips = null;
        }

        /// <summary>
        /// Gets pixel at given coordinates.
        /// </summary>
        /// <param name="x">The x coordinate.</param>
        /// <param name="y">The y coordinate.</param>
        /// <param name="mipLevel">The mip level.</param>
        /// <returns>The pixel at given coordinate.</returns>
        public Vector4 GetPixel(int x, int y, int mipLevel = 0)
        {
            Debug.Assert(x >= 0 && x < Width);
            Debug.Assert(y >= 0 && y < Height);
            Debug.Assert(mipLevel >= 0 && mipLevel < MipCount);
            Debug.Assert(MipCount > 0);
            Debug.Assert(Mips != null);
            Debug.Assert(Format == DXGI_FORMAT.R8G8B8A8_UNORM);

            var mip = Mips[mipLevel];
            var index = x * 4 + y * mip.Pitch;
            
            return new Vector4(mip.Data[index] / 255.0f, mip.Data[index + 1] / 255.0f, mip.Data[index + 2] / 255.0f, mip.Data[index + 3] / 255.0f);
        }

        protected override void OnSerializeBinary(ushort version, BinaryWriter writer)
        {
            Debug.Assert(MipCount > 0);

            // Write texture header
            writer.Write((uint)TotalSize);
            writer.Write((ushort)Format);
            writer.Write((byte)MipCount);
            
            // Write mips
            foreach(var mip in Mips)
            {
                Debug.Assert(mip.Width > 0);
                Debug.Assert(mip.Height > 0);
                Debug.Assert(mip.Pitch > 0);
                Debug.Assert(mip.Size > 0);

                writer.Write((ushort)mip.Width);
                writer.Write((ushort)mip.Height);
                writer.Write((ushort)mip.Pitch);
                writer.Write(mip.Size);
                writer.Write(mip.Data, 0, mip.Size);
            }
        }

        protected override void OnDeserializeBinary(ushort version, BinaryReader reader)
        {
            TotalSize = reader.ReadUInt32();
            Format = (DXGI_FORMAT)reader.ReadUInt16();
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

        public Mip[] Mips { get; set; }
        public DXGI_FORMAT Format { get; set; }
        public int MipCount { get; set; }
        public uint TotalSize { get; set; }

        public int Width => Mips[0].Width;
        public int Height => Mips[0].Height;
        public override AssetType AssetType => AssetType.Texture2D;

        public Texture Texture { get; private set; }
        public TextureView TextureView { get; private set; }
        public PixelFormat PixelFormat
        {
            get
            {
                switch (Format)
                {
                    case DXGI_FORMAT.R8_UNORM:
                        return PixelFormat.R8_UNorm;
                    case DXGI_FORMAT.R8_SNORM:
                        return PixelFormat.R8_SNorm;
                    case DXGI_FORMAT.R8_SINT:
                        return PixelFormat.R8_SInt;

                    case DXGI_FORMAT.R16_UNORM:
                        return PixelFormat.R16_UNorm;
                    case DXGI_FORMAT.R16_SNORM:
                        return PixelFormat.R16_SNorm;
                    case DXGI_FORMAT.R16_SINT:
                        return PixelFormat.R16_SInt;
                    case DXGI_FORMAT.R16_FLOAT:
                        return PixelFormat.R16_Float;

                    case DXGI_FORMAT.R32_UINT:
                        return PixelFormat.R32_UInt;
                    case DXGI_FORMAT.R32_SINT:
                        return PixelFormat.R32_SInt;

                    case DXGI_FORMAT.R8G8B8A8_UNORM:
                        return PixelFormat.R8_G8_B8_A8_UNorm;
                    case DXGI_FORMAT.R8G8B8A8_SNORM:
                        return PixelFormat.R8_G8_B8_A8_SNorm;
                    case DXGI_FORMAT.R8G8B8A8_UINT:
                        return PixelFormat.R8_G8_B8_A8_UInt;
                    case DXGI_FORMAT.R8G8B8A8_SINT:
                        return PixelFormat.R8_G8_B8_A8_SInt;

                    case DXGI_FORMAT.B8G8R8A8_UNORM:
                        return PixelFormat.B8_G8_R8_A8_UNorm;

                    case DXGI_FORMAT.R16G16B16A16_UNORM:
                        return PixelFormat.R16_G16_B16_A16_UNorm;
                    case DXGI_FORMAT.R16G16B16A16_SNORM:
                        return PixelFormat.R16_G16_B16_A16_SNorm;
                    case DXGI_FORMAT.R16G16B16A16_UINT:
                        return PixelFormat.R16_G16_B16_A16_UInt;
                    case DXGI_FORMAT.R16G16B16A16_SINT:
                        return PixelFormat.R16_G16_B16_A16_SInt;
                    case DXGI_FORMAT.R16G16B16A16_FLOAT:
                        return PixelFormat.R16_G16_B16_A16_Float;

                    case DXGI_FORMAT.R32G32B32A32_FLOAT:
                        return PixelFormat.R32_G32_B32_A32_Float;
                    case DXGI_FORMAT.R32G32B32A32_UINT:
                        return PixelFormat.R32_G32_B32_A32_UInt;
                    case DXGI_FORMAT.R32G32B32A32_SINT:
                        return PixelFormat.R32_G32_B32_A32_SInt;

                    case DXGI_FORMAT.BC1_UNORM:
                        return PixelFormat.BC1_Rgba_UNorm;
                    case DXGI_FORMAT.BC2_UNORM:
                        return PixelFormat.BC2_UNorm;
                    case DXGI_FORMAT.BC3_UNORM:
                        return PixelFormat.BC3_UNorm;
                    case DXGI_FORMAT.BC4_UNORM:
                        return PixelFormat.BC4_UNorm;
                    case DXGI_FORMAT.BC5_UNORM:
                        return PixelFormat.BC5_UNorm;
                    case DXGI_FORMAT.BC5_SNORM:
                        return PixelFormat.BC5_SNorm;
                    case DXGI_FORMAT.BC7_UNORM:
                        return PixelFormat.BC7_UNorm;

                    default:
                        throw new ArgumentOutOfRangeException($"Format {Format.ToString()} ({(int)Format}) is not supported for 2D textures!");
                }
            }
        }
    }
}
