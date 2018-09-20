// ReCrafted Editor (c) 2016-2018 Always Too Late

using System;
using System.IO;
using System.Text;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

namespace ReCrafted.Editor.Content
{
    public abstract class AssetBase : IAsset
    {
        private JsonWriter _currentWriter;
        private JObject _currentReaderObject;

        internal void SerializeToFile(string fileName, Formatting formatting = Formatting.None)
        {
            using (var fileStream = new FileStream(fileName, FileMode.Create, FileAccess.Write))
            {
                using (var sw = new StreamWriter(fileStream))
                {
                    // Make space for json length
                    fileStream.Position = 2;

                    // Serialize JSON
                    using (var jsonWriter = new JsonTextWriter(sw))
                    {
                        jsonWriter.CloseOutput = false;
                        jsonWriter.AutoCompleteOnClose = true;

                        _currentWriter = jsonWriter;
                        jsonWriter.Formatting = formatting;

                        jsonWriter.WriteStartObject();

                        SerializeInternal();
                        OnSerialize();

                        jsonWriter.WriteEndObject();

                    }

                }
            }

            // Finish writing
            using (var fileStream = new FileStream(fileName, FileMode.Open, FileAccess.Write))
            {
                // Write JSON length
                var length = fileStream.Length - 2;
                var lengthBytes = BitConverter.GetBytes((ushort)length);
                fileStream.Position = 0;
                fileStream.Write(lengthBytes, 0, lengthBytes.Length);


                // Write Binary
                if (HasBinaryData)
                {
                    // Set to the end
                    fileStream.Seek(0, SeekOrigin.End);

                    using (var bw = new BinaryWriter(fileStream))
                    {
                        OnSerializeBinary(bw);
                    }
                }
            }
        }

        internal void DeserializeFromFile(string fileName)
        {
            // Read JSON
            using (var fileStream = new FileStream(fileName, FileMode.Open, FileAccess.Read))
            {
                // Read length
                var lengthBytes = new[] { (byte)fileStream.ReadByte(), (byte)fileStream.ReadByte() };
                var jsonLength = BitConverter.ToUInt16(lengthBytes, 0);

                var jsonString = new StringBuilder();
                using (var binaryReader = new BinaryReader(fileStream))
                {
                    var jsonBytes = binaryReader.ReadBytes(jsonLength);
                    jsonString.Append(Encoding.UTF8.GetString(jsonBytes));

                    _currentReaderObject = JObject.Parse(jsonString.ToString(), new JsonLoadSettings
                    {
                        CommentHandling = CommentHandling.Ignore,
                        LineInfoHandling = LineInfoHandling.Ignore
                    });

                    DeserializeInternal();
                    OnDeserialize();

                    // Read binary
                    if (HasBinaryData)
                    {
                        OnDeserializeBinary(binaryReader);
                    }
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
