using ReCrafted.Graphics;

namespace ReCrafted.Voxels
{
    /// <summary>
    /// VoxelAssets class.
    /// Contains all needed assets for the voxel engine.
    /// </summary>
    public static class VoxelAssets
    {
        /// <summary>
        /// Load all assets.
        /// </summary>
        internal static void LoadAssets()
        {
            DefaultShader = Shader.FromFile("default_block");
            ColorShader = Shader.FromFile("color");

            DefaultAtlas = Texture2D.FromFile("assets/textures/block_atlas.png");
        }

        /// <summary>
        /// Unload all assets.
        /// </summary>
        internal static void UnloadAssets()
        {
            DefaultShader?.Dispose();
            ColorShader?.Dispose();
            DefaultAtlas?.Dispose();
        }

        /// <summary>
        /// Default shader.
        /// </summary>
        public static Shader DefaultShader { get; set; }

        /// <summary>
        /// Color-only shader.
        /// </summary>
        public static Shader ColorShader { get; set; }

        /// <summary>
        /// Default blocks atlas.
        /// </summary>
        public static Texture2D DefaultAtlas { get; set; }
    }
}