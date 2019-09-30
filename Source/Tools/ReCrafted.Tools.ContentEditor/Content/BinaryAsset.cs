// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using System.IO;

namespace ReCrafted.Tools.ContentEditor.Content
{
    public abstract class BinaryAsset : Asset
    {
        protected abstract override void OnSerializeBinary(ushort version, BinaryWriter writer);
        protected abstract override void OnDeserializeBinary(ushort version, BinaryReader reader);

        protected override AssetBaseType AssetBaseType => AssetBaseType.Binary;
    }
}
