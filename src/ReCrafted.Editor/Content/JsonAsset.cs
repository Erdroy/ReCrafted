// ReCrafted Editor (c) 2016-2018 Always Too Late

namespace ReCrafted.Editor.Content
{
    public abstract class JsonAsset : Asset
    {
        protected abstract override void OnSerializeJson(ushort version);
        protected abstract override void OnDeserializeJson(ushort version);

        protected override AssetBaseType AssetBaseType => AssetBaseType.Json;
    }
}
