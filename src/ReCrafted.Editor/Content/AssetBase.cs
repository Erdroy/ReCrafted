// ReCrafted Editor (c) 2016-2018 Always Too Late

using System;
using System.Collections.Generic;
using System.IO;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

namespace ReCrafted.Editor.Content
{
    public abstract class AssetBase : IAsset
    {
        private JsonWriter _currentWriter;
        private JObject _currentReaderObject;

        public void Serialize(FileStream stream, Formatting formatting = Formatting.None)
        {
            var sw = new StreamWriter(stream);
            
            // Write JSON
            using (var jsonWriter = new JsonTextWriter(sw))
            {
                jsonWriter.CloseOutput = false;
                
                _currentWriter = jsonWriter;
                jsonWriter.Formatting = formatting;

                jsonWriter.WriteStartObject();

                SerializeInternal();
                OnSerialize();

                jsonWriter.WriteEndObject();
            }

            // Write Binary
            if (HasBinaryData)
            {
                using (var bw = new BinaryWriter(stream))
                {
                    OnSerializeBinary(bw);
                }
            }
        }

        public void Deserialize(FileStream stream)
        {
            // Read JSON
            using (var sr = new StreamReader(stream))
            {
                using (var jsonReader = new JsonTextReader(sr))
                {
                    _currentReaderObject = JObject.Load(jsonReader, new JsonLoadSettings
                    {
                        CommentHandling = CommentHandling.Ignore,
                        LineInfoHandling = LineInfoHandling.Ignore
                    });

                    DeserializeInternal();
                    OnDeserialize();
                }
            }

            // Read Binary
            if (HasBinaryData)
            {
                using (var br = new BinaryReader(stream))
                {
                    OnDeserializeBinary(br);
                }
            }
        }

        protected virtual void OnSerialize() { }

        protected virtual void OnDeserialize() { }

        protected virtual void OnSerializeBinary(BinaryWriter writer) { }

        protected virtual void OnDeserializeBinary(BinaryReader reader)  { }

        protected void SerializeField<T>(string name, T value)
        {
            _currentWriter.WritePropertyName(name);
            _currentWriter.WriteValue(value);
        }

        protected T DeserializeField<T>(string name, T defaultValue = default(T))
        {
            if (!_currentReaderObject.TryGetValue(name, out var value))
                return defaultValue;

            return value.Value<T>();
        }

        private void SerializeInternal()
        {
            SerializeField("Guid", Guid.NewGuid()); // Temporary
            SerializeField("Type", AssetType.ToString());
        }

        private void DeserializeInternal()
        {

        }

        public abstract void Unload();

        public abstract bool HasBinaryData { get; }
        public abstract AssetType AssetType { get; }
    }
}
