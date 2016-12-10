// ReCrafted © 2016 Damian 'Erdroy' Korczowski

using LibNoise;
using SharpDX;

namespace ReCrafted.Voxels.Generator
{
    public static class VoxelWorldGenerator
    {
        private static Perlin _perlinNoise;

        public static ushort GenerateTerrains(Int3 pos)
        {
            if (_perlinNoise == null)
            {
                _perlinNoise = new Perlin
                {
                    Frequency = 0.1f,
                    Persistence = 0.3f, 
                    OctaveCount = 3,
                    Lacunarity = 0.3f,
                    NoiseQuality = NoiseQuality.Standard,

                    Seed = 1001
                };
            }

            var baseHeight = 40.0f;

            var noiseA = _perlinNoise.GetValue(pos.X, pos.Y, pos.Z);
            var noiseB = _perlinNoise.GetValue(pos.X / 10.0f, pos.Y / 10.0f, pos.Z / 10.0f);

            // hill height
            noiseA *= 3.0f;
            noiseB *= 15;
            
            // base height
            noiseA += baseHeight;
            noiseB += baseHeight;

            var noise = noiseA*0.5f + noiseB*0.5f;

            ushort block = 0;
            
            if (noise - 3 >= pos.Y)
            {
                block = 1;
            }
            else if (noise - 2 >= pos.Y)
            {
                block = 2;
            }
            else if (noise >= pos.Y)
            {
                block = 4;
            }
            
            return block;
        }

        public static ushort GenerateCaves(ushort block, Int3 pos)
        {

            return block;
        }
    }
}