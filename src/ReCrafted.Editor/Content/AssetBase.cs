// ReCrafted Editor (c) 2016-2018 Always Too Late

using Newtonsoft.Json;

namespace ReCrafted.Editor.Content
{
    public abstract class AssetBase : IAsset
    {
        public void SerializeField<T>(string name, T value)
        {

        }

        public T DeserializeField<T>(string name)
        {
            return default(T);
        }

        public virtual void Serialize()
        {
        }

        public virtual void Deserialize()
        {
        }

        public abstract void Unload();

        public AssetType AssetType { get; protected set; }
    }
}
