// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

namespace ReCrafted.Tools.ContentEditor.Content.Assets
{
    /// <summary>
    ///     VoxelObjectAsset asset class.
    /// </summary>
    public class VoxelObjectAsset : JsonAsset
    {
        protected override void OnSerializeJson(ushort version)
        {
            SerializeField("TopHeightMap", TopHeightMap);
            SerializeField("BottomHeightMap", BottomHeightMap);
            SerializeField("LeftHeightMap", LeftHeightMap);
            SerializeField("RightHeightMap", RightHeightMap);
            SerializeField("FrontHeightMap", FrontHeightMap);
            SerializeField("BackHeightMap", BackHeightMap);
        }

        protected override void OnDeserializeJson(ushort version)
        {
            TopHeightMap = DeserializeField("TopHeightMap", "Unknown");
            BottomHeightMap = DeserializeField("BottomHeightMap", "Unknown");
            LeftHeightMap = DeserializeField("LeftHeightMap", "Unknown");
            RightHeightMap = DeserializeField("RightHeightMap", "Unknown");
            FrontHeightMap = DeserializeField("FrontHeightMap", "Unknown");
            BackHeightMap = DeserializeField("BackHeightMap", "Unknown");
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
