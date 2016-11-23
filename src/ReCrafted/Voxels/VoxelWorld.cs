using System;
using ReCrafted.Core;

namespace ReCrafted.Voxels
{
    public sealed class VoxelWorld : IDisposable, IRendererComponent
    {
        public static VoxelWorld Instance;

        private VoxelChunk _chunk;

        public VoxelWorld()
        {
            Instance = this;
        }

        public void Init()
        {
            VoxelAssets.LoadAssets();

            _chunk = new VoxelChunk();
            _chunk.Init();
        }

        public void Tick()
        {
            _chunk.Tick();
        }

        public void Simulate()
        {
        }

        public void Draw()
        {
            _chunk.Draw();
        }

        public void Dispose()
        {
            _chunk.Dispose();
        }
    }
}