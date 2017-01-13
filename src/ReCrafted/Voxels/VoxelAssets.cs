// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

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
        /// <summary>
        /// Contains all blocks descriptors with id's.
        /// </summary>
        public static Dictionary<ushort, VoxelBlock> Blocks = new Dictionary<ushort, VoxelBlock>();

        /// <summary>
        /// Load all assets.
        /// </summary>
        internal static void LoadAssets()
        {
            DefaultShader = Shader.FromFile("render_gbuffer");
            
            DefaultSampler = Sampler.Create(Sampler.Type.PointClamped);

            // Add default blocks
            AddBlock(1, new VoxelBlock
            {
                Name = "stone",
                Texture = Texture2D.FromFile("assets/textures/blocks/stone.png", true)
            });

            AddBlock(2, new VoxelBlock
            {
                Name = "dirt",
                Texture = Texture2D.FromFile("assets/textures/blocks/dirt.png", true)
            });

            AddBlock(3, new VoxelBlock
            {
                Name = "grass",
                Texture = Texture2D.FromFile("assets/textures/blocks/dirt.png", true)
                // TODO: Side overlays, custom top/bottom texture etc.
            });

            AddBlock(4, new VoxelBlock
            {
                Name = "sand",
                Texture = Texture2D.FromFile("assets/textures/blocks/sand.png", true)
            });

            AddBlock(5, new VoxelBlock
            {
                Name = "bedrock",
                Texture = Texture2D.FromFile("assets/textures/blocks/bedrock.png", true)
            });

            AddBlock(6, new VoxelBlock
            {
                Name = "planks",
                Texture = Texture2D.FromFile("assets/textures/blocks/planks.png", true)
            });

            AddBlock(20, new VoxelBlock
            {
                Name = "coal ore",
                Texture = Texture2D.FromFile("assets/textures/blocks/coal_ore.png", true)
            });

            AddBlock(21, new VoxelBlock
            {
                Name = "iron ore",
                Texture = Texture2D.FromFile("assets/textures/blocks/iron_ore.png", true)
            });

            AddBlock(22, new VoxelBlock
            {
                Name = "gold ore",
                Texture = Texture2D.FromFile("assets/textures/blocks/gold_ore.png", true)
            });
        }

        /// <summary>
        /// Unload all assets.
        /// </summary>
        internal static void UnloadAssets()
        {
            DefaultShader?.Dispose();
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
        /// Default sampler state.
        /// </summary>
        public static Sampler DefaultSampler { get; set; }
    }
}