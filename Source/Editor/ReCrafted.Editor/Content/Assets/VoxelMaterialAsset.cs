// ReCrafted (c) 2016-2019 Always Too Late

namespace ReCrafted.Editor.Content.Assets
{
    public class VoxelMaterialAsset : JsonAsset
    {
        protected override void OnSerializeJson(ushort version)
        {
            SerializeField("VoxelName", VoxelName);
            SerializeField("VoxelMaterial", VoxelMaterial);
            SerializeField("VoxelHardness", VoxelHardness);

            SerializeField("VoxelTexture_CB", TextureColorBlend);
            SerializeField("VoxelTexture_NSM", TextureNormalSmootnessMetallic);
            SerializeField("VoxelTexture_CB_Far", TextureColorBlendFar);
            SerializeField("VoxelTexture_NSM_Far", TextureNormalSmootnessMetallicFar);
        }

        protected override void OnDeserializeJson(ushort version)
        {
            VoxelName = DeserializeField("VoxelName", "Unknown");
            VoxelMaterial = DeserializeField<ushort>("VoxelMaterial");
            VoxelHardness = DeserializeField<byte>("VoxelHardness");

            TextureColorBlend = DeserializeField("VoxelTexture_CB", "Textures/Voxels/DefaultCB");
            TextureNormalSmootnessMetallic = DeserializeField("VoxelTexture_NSM", "Textures/Voxels/DefaultNSM");
            TextureColorBlendFar = DeserializeField("VoxelTexture_CB_Far", "Textures/DefaultCB");
            TextureNormalSmootnessMetallicFar = DeserializeField("VoxelTexture_NSM_Far", "Textures/Voxels/DefaultNSM");
        }

        public string VoxelName { get; set; }
        public ushort VoxelMaterial { get; set; }
        public byte VoxelHardness { get; set; }

        public string TextureColorBlend { get; set; }
        public string TextureNormalSmootnessMetallic { get; set; }
        public string TextureColorBlendFar { get; set; }
        public string TextureNormalSmootnessMetallicFar { get; set; }

        public override AssetType AssetType => AssetType.VoxelMaterial;
    }
}
