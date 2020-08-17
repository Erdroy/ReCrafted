// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

using System.IO;
using ReCrafted.Tools.ContentEditor.Core;

namespace ReCrafted.Tools.ContentEditor.Content.Assets
{
    /// <summary>
    ///     VoxelObjectAsset asset class.
    /// </summary>
    public class VoxelObjectAsset : BinaryAsset
    {
        protected override void OnSerializeBinary(ushort version, BinaryWriter writer)
        {
            writer.Write(ObjectName);
            writer.Write(ObjectType);

            writer.Write(MinimalSurfaceHeight);
            writer.Write(MaximalSurfaceHeight);
            writer.Write(HillsHeight);
            writer.Write(InitialOctreeDepth);

            if (ObjectType == 1)
            {
                // Write CHM data
                Heightmap.HeightmapFromTextures(
                    writer,
                    TopHeightMap,
                    BottomHeightMap,
                    LeftHeightMap,
                    RightHeightMap,
                    FrontHeightMap,
                    BackHeightMap
                );
            }
        }

        protected override void OnDeserializeBinary(ushort version, BinaryReader reader)
        {
            ObjectName = reader.ReadString();
            ObjectType = reader.ReadInt32();

            MinimalSurfaceHeight = reader.ReadInt32();
            MaximalSurfaceHeight = reader.ReadInt32();
            HillsHeight = reader.ReadInt32();
            InitialOctreeDepth = reader.ReadInt32();

            // Note: We're skipping the CHM/PreGen data here,
            // because we do not need it in the editor.
        }

        public string ObjectName { get; set; }
        public int ObjectType { get; set; }
        public int MinimalSurfaceHeight { get; set; }
        public int MaximalSurfaceHeight { get; set; }
        public int HillsHeight { get; set; }
        public int InitialOctreeDepth { get; set; }

        public string TopHeightMap { get; set; }
        public string BottomHeightMap { get; set; }
        public string LeftHeightMap { get; set; }
        public string RightHeightMap { get; set; }
        public string FrontHeightMap { get; set; }
        public string BackHeightMap { get; set; }

        public override AssetType AssetType => AssetType.VoxelObject;
    }
}
