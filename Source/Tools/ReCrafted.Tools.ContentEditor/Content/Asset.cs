// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using System;
using System.IO;
using System.Linq;
using System.Text;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

namespace ReCrafted.Tools.ContentEditor.Content
{
    /// <summary>
    ///     AssetBase class. Base class for all ReCrafted assets.
    /// </summary>
    public abstract class Asset
    {
        private const ushort AssetVersion = 1;
        private static readonly byte[] AssetMagic = Encoding.ASCII.GetBytes("RCA");

        private JObject _currentObject;
        private JsonTextWriter _currentWriter;

        /// <summary>
        ///     Guid of this asset.
        /// </summary>
        public Guid AssetGuid { get; set; }

        /// <summary>
        ///     Type of this asset.
        /// </summary>
        public abstract AssetType AssetType { get; }

        /// <summary>
        ///     The base type of this asset.
        /// </summary>
        protected abstract AssetBaseType AssetBaseType { get; }

        /// <summary>
        ///     Called when asset needs to be initialized.
        /// </summary>
        public virtual void Initialize()
        {
        }

        /// <summary>
        ///     Unloads this asset.
        /// </summary>
        public virtual void Unload()
        {
        }

        /// <summary>
        ///     Called when this asset needs binary serialization.
        /// </summary>
        /// <param name="version">The current asset version.</param>
        /// <param name="writer">The writer stream.</param>
        protected virtual void OnSerializeBinary(ushort version, BinaryWriter writer)
        {
        }

        /// <summary>
        ///     Called when this asset needs binary deserialization.
        /// </summary>
        /// <param name="version">The current asset version.</param>
        /// <param name="reader">The reader stream.</param>
        protected virtual void OnDeserializeBinary(ushort version, BinaryReader reader)
        {
        }

        /// <summary>
        ///     Called when this asset needs JSON serialization.
        ///     Use SerializeField to write JSON data.
        /// </summary>
        /// <param name="version">The current asset version.</param>
        protected virtual void OnSerializeJson(ushort version)
        {
        }

        /// <summary>
        ///     Called when this asset needs JSON deserialization.
        ///     Use DeserializeField to read JSON data.
        /// </summary>
        /// <param name="version">The current asset version.</param>
        protected virtual void OnDeserializeJson(ushort version)
        {
        }

        protected void SerializeField<T>(string name, T value)
        {
            _currentWriter.WritePropertyName(name);
            _currentWriter.WriteValue(value);
        }

        protected T DeserializeField<T>(string name, T defaultValue = default)
        {
            if (!_currentObject.ContainsKey(name))
                return defaultValue;
            return _currentObject[name].ToObject<T>();
        }

        private void InternalSerialize(BinaryWriter writer)
        {
            writer.Write(AssetMagic); // Always 3 bytes!
            writer.Write(AssetVersion);
            writer.Write((byte) AssetType);
            writer.Write(AssetGuid.ToByteArray());
        }

        private ushort InternalDeserialize(BinaryReader reader)
        {
            var magic = reader.ReadBytes(AssetMagic.Length);
            if (!magic.SequenceEqual(AssetMagic))
                throw new Exception("This is not a valid ReCrafted asset!");

            var version = reader.ReadUInt16();
            var assetType = reader.ReadByte();
            AssetGuid = new Guid(reader.ReadBytes(Guid.Empty.ToByteArray().Length));

            if (assetType != (byte) AssetType)
                throw new Exception(
                    $"Invalid asset importer used. Using importer of {AssetType} asset to import {(AssetType) assetType} asset.");

            return version;
        }

        private void InternalSerializeJson()
        {
            SerializeField("AssetVersion", AssetVersion);
            SerializeField("AssetType", (byte) AssetType);
            SerializeField("AssetGuid", AssetGuid);
        }

        private ushort InternalDeserializeJson()
        {
            var version = DeserializeField("AssetVersion", ushort.MaxValue);
            var assetType = DeserializeField("AssetType", byte.MaxValue);
            AssetGuid = Guid.Parse(DeserializeField<string>("AssetGuid"));

            if (assetType != (byte) AssetType)
                throw new Exception(
                    $"Invalid asset importer used. Using importer of {AssetType} to import {(AssetType) assetType}.");

            OnDeserializeJson(version);
            return version;
        }

        internal void Serialize(Stream stream)
        {
            switch (AssetBaseType)
            {
                case AssetBaseType.Unknown:
                    break;
                case AssetBaseType.Binary:
                {
                    using (var binaryWriter = new BinaryWriter(stream))
                    {
                        InternalSerialize(binaryWriter);
                        OnSerializeBinary(AssetVersion, binaryWriter);
                    }

                    break;
                }

                case AssetBaseType.Json:
                {
                    using (var sw = new StreamWriter(stream))
                    {
                        using (var jw = new JsonTextWriter(sw))
                        {
                            _currentWriter = jw;

                            jw.WriteStartObject();

                            InternalSerializeJson();
                            OnSerializeJson(AssetVersion);

                            jw.WriteEndObject();
                        }
                    }

                    break;
                }

                default:
                    throw new ArgumentOutOfRangeException();
            }
        }

        internal void Deserialize(Stream stream)
        {
            switch (AssetBaseType)
            {
                case AssetBaseType.Unknown:
                    break;
                case AssetBaseType.Binary:
                {
                    using (var binaryReader = new BinaryReader(stream))
                    {
                        var version = InternalDeserialize(binaryReader);
                        OnDeserializeBinary(version, binaryReader);
                    }

                    break;
                }

                case AssetBaseType.Json:
                {
                    using (var sr = new StreamReader(stream))
                    {
                        using (var jr = new JsonTextReader(sr))
                        {
                            _currentObject = JObject.Load(jr);
                            var version = InternalDeserializeJson();
                            OnDeserializeJson(version);
                        }
                    }

                    break;
                }

                default:
                    throw new ArgumentOutOfRangeException();
            }
        }
    }
}