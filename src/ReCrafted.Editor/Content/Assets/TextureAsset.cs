// ReCrafted Editor (c) 2016-2018 Always Too Late

using System.IO;

namespace ReCrafted.Editor.Content.Assets
{
    public class TextureAsset : AssetBase
    {
        protected override void OnSerialize()
        {
            SerializeField("Test", Test);
        }

        protected override void OnDeserialize()
        {
            Test = DeserializeField<bool>("Test", true);
        }

        protected override void OnSerializeBinary(BinaryWriter writer)
        {
            writer.Write("Hello, World!");
        }

        protected override void OnDeserializeBinary(BinaryReader reader)
        {
            var footprint = reader.ReadString();
        }

        public override void Unload()
        {
            
        }

        public bool Test { get; set; }

        public override bool HasBinaryData => true;
        public override AssetType AssetType => AssetType.Texture;
    }
}
