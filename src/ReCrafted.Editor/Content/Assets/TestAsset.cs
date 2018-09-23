// ReCrafted Editor (c) 2016-2018 Always Too Late

using System;
using System.IO;

namespace ReCrafted.Editor.Content.Assets
{
    public class TestAsset : AssetBase
    {
        protected override void OnSerialize(ushort version, BinaryWriter writer)
        {
            writer.Write(0xFFAAFFAAUL);
        }

        protected override void OnDeserialize(ushort version, BinaryReader reader)
        {
            if(reader.ReadUInt64() != 0xFFAAFFAAUL)
                throw new Exception("ERROR - Not a TestAsset!");
        }

        public override void Unload()
        {

        }

        public override AssetType AssetType => AssetType.Test;
    }
}
