// ReCrafted © 2016 Damian 'Erdroy' Korczowski

using System.Collections.Generic;
using ReCrafted.Graphics;

namespace ReCrafted.Voxels
{
    /// <summary>
    /// VoxelAssets class.
    /// Contains all needed assets for the voxel engine.
    /// </summary>
    public static class VoxelAssets
    {
        public static Dictionary<ushort, VoxelBlock> Blocks = new Dictionary<ushort, VoxelBlock>();

        /// <summary>
        /// Load all assets.
        /// </summary>
        internal static void LoadAssets()
        {
            DefaultShader = Shader.FromFile("default_block");
            ColorShader = Shader.FromFile("color");
            
            DefaultSampler = Sampler.Create(Sampler.Type.PointClamped);

            // Add default blocks
            AddBlock(1, new VoxelBlock
            {
                Name = "stone",
                Texture = Texture2D.FromFile("assets/textures/blocks/stone.png")
            });

            AddBlock(2, new VoxelBlock
            {
                Name = "dirt",
                Texture = Texture2D.FromFile("assets/textures/blocks/dirt.png")
            });

            AddBlock(3, new VoxelBlock
            {
                Name = "grass",
                Texture = Texture2D.FromFile("assets/textures/blocks/dirt.png")
                // TODO: Side overlays, custom top/bottom texture etc.
            });

            AddBlock(4, new VoxelBlock
            {
                Name = "sand",
                Texture = Texture2D.FromFile("assets/textures/blocks/sand.png")
            });

            AddBlock(5, new VoxelBlock
            {
                Name = "bedrock",
                Texture = Texture2D.FromFile("assets/textures/blocks/bedrock.png")
            });
        }

        /// <summary>
        /// Unload all assets.
        /// </summary>
        internal static void UnloadAssets()
        {
            DefaultShader?.Dispose();
            ColorShader?.Dispose();
        }

        /// <summary>
        /// Adds new voxel block.
        /// </summary>
        /// <param name="blockId">The block id.</param>
        /// <param name="block">The block.</param>
        public static void AddBlock(ushort blockId, VoxelBlock block)
        {
            Blocks.Add(blockId, block);
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
        /// Default sampler state.
        /// </summary>
        public static Sampler DefaultSampler { get; set; }
    }
}