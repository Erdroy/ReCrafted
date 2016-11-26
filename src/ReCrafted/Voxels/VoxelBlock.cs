
using ReCrafted.Graphics;

namespace ReCrafted.Voxels
{
    /// <summary>
    /// VoxelBlock class.
    /// </summary>
    public class VoxelBlock
    {
        /// <summary>
        /// Name of the block.
        /// </summary>
        public string Name { get; set; }

        /// <summary>
        /// This block has top texture?
        /// </summary>
        public bool HasTopTexture { get; set; }

        /// <summary>
        /// This block has bottom texture?
        /// </summary>
        public bool HasBottomTexture { get; set; }

        /// <summary>
        /// This block has side overlay?
        /// </summary>
        public bool HasSideOverlay { get; set; }

        /// <summary>
        /// This block has top overlay?
        /// </summary>
        public bool HasTopOverlay { get; set; }
        
        /// <summary>
        /// This block has bottom overlay?
        /// </summary>
        public bool HasBottomOverlay { get; set; }
        
        /// <summary>
        /// The default texture of this block.
        /// </summary>
        public Texture2D Texture { get; set; }

        /// <summary>
        /// The top texture.
        /// Optional - must be used when HasTopTexture is true.
        /// </summary>
        public Texture2D TopTexture { get; set; }

        /// <summary>
        /// The bottom texture.
        /// Optional - must be used when HasBottomTexture is true.
        /// </summary>
        public Texture2D BottomTexture { get; set; }

        /// <summary>
        /// The side overlay.
        /// Optional - must be used when HasSideOverlay is true.
        /// </summary>
        public Texture2D SideOverlay { get; set; }

        /// <summary>
        /// The top overlay.
        /// Optional - must be used when HasTopOverlay is true.
        /// </summary>
        public Texture2D TopOverlay { get; set; }

        /// <summary>
        /// The bottom overlay.
        /// Optional - must be used when HasBottomOverlay is true.
        /// </summary>
        public Texture2D BottomOverlay { get; set; }

        /// <summary>
        /// Custom shader reference.
        /// </summary>
        public Shader CustomShader { get; set; }
    }
}