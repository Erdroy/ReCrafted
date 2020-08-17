// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

using System.IO;
using DirectXTexNet;
using ReCrafted.Tools.ContentEditor.Content.Assets;

namespace ReCrafted.Tools.ContentEditor.Core
{
    public static class Heightmap
    {
        private static TextureAsset LoadTexture(string textureName)
        {
            var texture = new TextureAsset();

            using (var fs = new FileStream(textureName, FileMode.Open, FileAccess.Read))
            {
                texture.Deserialize(fs);
            }

            // Convert to R8 UINT
            texture.Convert(DXGI_FORMAT.R8_UINT);

            return texture;
        }

        public static void HeightmapFromTextures(BinaryWriter stream, string top, string bottom, string left, string right, string front, string back)
        {
            // Load all textures
            var textureFront = LoadTexture(front);
            var textureBack = LoadTexture(back);
            var textureLeft = LoadTexture(left);
            var textureRight = LoadTexture(right);
            var textureTop = LoadTexture(top);
            var textureBottom = LoadTexture(bottom);

            var format = textureFront.Format; // Must be R8 or R16
            var mips = textureFront.MipCount;
            var width = textureFront.Width;
            var height = textureFront.Height; // Should be the same as width

            // TODO: Validate all texture properties - are these the same?

            var is16Bit = format != DXGI_FORMAT.R8_UINT;
            var texelSize = is16Bit ? 2 : 1;

            // Write metadata
            stream.Write(width);
            stream.Write(height);
            stream.Write(is16Bit); // Is16Bit - true when R16, false when R8
            stream.Write(mips);
            stream.Write(true); // IsCubical - true when cubical (CHM), false when flat (HM)
            // Note: We don't have support for flat height maps.

            // Write mips
            for (var mipId = 0; mipId < mips; mipId++)
            {
                // Get the front texture as the reference
                var referenceMip = textureFront.Mips[mipId];

                // Face order: Front, back, left, right, top, bottom
                var mipSize = referenceMip.Width * referenceMip.Height * texelSize;

                // Write all faces
                stream.Write(textureFront.Mips[mipId].Data, 0, mipSize); // Front
                stream.Write(textureBack.Mips[mipId].Data, 0, mipSize); // Back
                stream.Write(textureLeft.Mips[mipId].Data, 0, mipSize); // Left
                stream.Write(textureRight.Mips[mipId].Data, 0, mipSize); // Right
                stream.Write(textureTop.Mips[mipId].Data, 0, mipSize); // Top
                stream.Write(textureBottom.Mips[mipId].Data, 0, mipSize); // Bottom
            }
        }
    }
}
