
using ReCrafted.Graphics;

namespace ReCrafted.Voxels
{
    /// <summary>
    /// VoxelBlock class.
    /// </summary>
    public class VoxelBlock
    {
        public string Name { get; set; }

        public bool HasTopTexture { get; set; }

        public bool HasBottomTexture { get; set; }

        public Texture2D Texture { get; set; }

        public Texture2D TopTexture { get; set; }

        public Texture2D BottomTexture { get; set; }

        public Shader CustomShader { get; set; }
    }
}