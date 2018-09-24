// ReCrafted Editor (c) 2016-2018 Always Too Late

using System.IO;

namespace ReCrafted.Editor.Content
{
    public abstract class BinaryAsset : Asset
    {
        protected abstract override void OnSerializeBinary(ushort version, BinaryWriter writer);
        protected abstract override void OnDeserializeBinary(ushort version, BinaryReader reader);

        protected override AssetBaseType AssetBaseType => AssetBaseType.Binary;
    }
}
