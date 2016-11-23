using ReCrafted.Graphics;

namespace ReCrafted.Voxels
{
    public static class VoxelAssets
    {
        internal static void LoadAssets()
        {
            DefaultShader = Shader.FromFile("default_block");

            DefaultAtlas = Texture2D.FromFile("assets/textures/block_atlas.png");
        }

        public static Shader DefaultShader { get; set; }

        public static Texture2D DefaultAtlas { get; set; }
    }
}