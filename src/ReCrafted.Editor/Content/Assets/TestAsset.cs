// ReCrafted Editor (c) 2016-2018 Always Too Late

namespace ReCrafted.Editor.Content.Assets
{
    public class TestAsset : JsonAsset
    {
        protected override void OnSerializeJson(ushort version)
        {
        }

        protected override void OnDeserializeJson(ushort version)
        {
        }

        public override void Unload()
        {

        }

        public override AssetType AssetType => AssetType.Test;
    }
}
