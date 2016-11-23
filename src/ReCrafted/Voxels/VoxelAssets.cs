using ReCrafted.Graphics;

namespace ReCrafted.Voxels
{
    public static class VoxelAssets
    {
        internal static void LoadAssets()
        {
            DefaultShader = Shader.FromFile("default_block");
        }

        public static Shader DefaultShader { get; set; }
    }
}