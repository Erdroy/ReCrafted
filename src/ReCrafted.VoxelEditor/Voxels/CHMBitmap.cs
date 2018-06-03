// ReCrafted (c) 2016-2018 Always Too Late

using System;
using System.IO;

namespace ReCrafted.VoxelEditor.Voxels
{
    public class CHMBitmap : IDisposable
    {
        public enum FaceType
        {
            Front,
            Back,
            Left,
            Right,
            Top,
            Bottom,
        }

        private struct FaceBitmap
        {
            public byte[] Bitmap;
            public int Width;
            public int Height;
            
            public FaceBitmap(byte[] bitmap, int width, int height)
            {
                Bitmap = bitmap;
                Width = width;
                Height = height;
            }
        }

        private class Face
        {
            public readonly FaceBitmap[] MipMaps;

            public Face(int mipMapCount)
            {
                MipMaps = new FaceBitmap[mipMapCount];
            }
        }

        private readonly int _width;
        private readonly int _height;
        private readonly int _mipMapCount;
        private readonly Face[] _faces = new Face[6];

        public CHMBitmap(int width, int height, int mipMapCount)
        {
            _width = width;
            _height = height;
            _mipMapCount = mipMapCount;
        }

        public void AddFaceMip(FaceType faceType, int mipLevel, byte[] data, int width, int height)
        {
            var face = _faces[(int) faceType] ?? (_faces[(int)faceType] = new Face(_mipMapCount));

            // Insert mip level
            face.MipMaps[mipLevel] = new FaceBitmap(data, width, height);
        }

        public void Save(string fileName)
        {
            using (var ms = new MemoryStream())
            {
                using (var bw = new BinaryWriter(ms))
                {
                    // Build header
                    // Signature
                    bw.Write('R'); // RCCHM10
                    bw.Write('C');
                    bw.Write('C');
                    bw.Write('H');
                    bw.Write('M');
                    bw.Write('1');
                    bw.Write('0');
                    bw.Write('\0');

                    // Description
                    bw.Write((ushort)_width);
                    bw.Write((ushort)_height);
                    bw.Write((ushort)0);
                    bw.Write((byte)_mipMapCount);
                    bw.Write((byte)0);

                    // Save all mips
                    for (var i = 0; i < _mipMapCount; i++)
                    {
                        for (var j = 0; j < 6; j++)
                        {
                            var mip = _faces[j].MipMaps[i];

                            // Write bitmap data
                            bw.Write(mip.Bitmap, 0, mip.Width * mip.Height);
                        }
                    }

                    // Write out the data
                    File.WriteAllBytes(fileName, ms.ToArray());
                }
            }

        }

        public void Dispose()
        {
        }

        public static CHMBitmap Create(int baseWidth, int baseHeight, int mipMapCount)
        {
            return new CHMBitmap(baseWidth, baseHeight, mipMapCount);
        }

        public static CHMBitmap CreateFromFile(string fileName)
        {
            return new CHMBitmap(0, 0, 0);
        }
    }
}
