// ReCrafted Editor (c) 2016-2018 Always Too Late

using System;
using System.IO;
using System.Text;

namespace ReCrafted.Editor.Content
{
    /// <summary>
    /// AssetBase class. Base class for all ReCrafted assets.
    /// </summary>
    public abstract class AssetBase : IAsset
    {
        private static readonly byte[] AssetMagic = Encoding.ASCII.GetBytes("RCA");
        private const ushort AssetVersion = 1;

        /// <summary>
        /// Called when this asset needs serialization.
        /// </summary>
        /// <param name="version">The current asset version.</param>
        /// <param name="writer">The writer stream.</param>
        protected virtual void OnSerialize(ushort version, BinaryWriter writer) { }

        /// <summary>
        /// Called when this asset needs deserialization.
        /// </summary>
        /// <param name="version">The current asset version.</param>
        /// <param name="reader">The reader stream.</param>
        protected virtual void OnDeserialize(ushort version, BinaryReader reader) { }

        private void InternalSerialize(BinaryWriter writer)
        {
            writer.Write(AssetMagic); // Always 3 bytes!
            writer.Write(AssetVersion);
            writer.Write((byte)AssetType);
        }

        private ushort InternalDeserialize(BinaryReader reader)
        {
            var magic = reader.ReadBytes(AssetMagic.Length);
            if (magic != AssetMagic)
                throw new Exception("This is not a valid ReCrafted asset!");

            var version = reader.ReadUInt16();
            var assetType = reader.ReadByte();

            if (assetType != (byte)AssetType)
                throw new Exception($"Invalid asset importer used. Using importer of {AssetType} to import {(AssetType)assetType}.");

            return version;
        }

        internal void Serialize(Stream stream)
        {
            using (var binaryWriter = new BinaryWriter(stream))
            {
                InternalSerialize(binaryWriter);
                OnSerialize(AssetVersion, binaryWriter);
            }
        }

        internal void Deserialize(Stream stream)
        {
            using (var binaryReader = new BinaryReader(stream))
            {
                var version = InternalDeserialize(binaryReader);
                OnDeserialize(version, binaryReader);
            }
        }
        
        /// <summary>
        /// Unloads this asset.
        /// </summary>
        public abstract void Unload();
        
        /// <summary>
        /// Type of this asset.
        /// </summary>
        public abstract AssetType AssetType { get; }
    }
}
