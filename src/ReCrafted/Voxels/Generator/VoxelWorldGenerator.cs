// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

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
                    OctaveCount = 1,
                    Lacunarity = 0.3f,
                    NoiseQuality = NoiseQuality.Low,

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
            
            if (noise - 4 >= pos.Y)
            {
                block = 1;
            }
            else if (noise - 3 >= pos.Y)
            {
                block = 2;
            }
            else if (noise >= pos.Y)
            {
                block = 4;
            }


            // generate ores
            var coalNoise = _perlinNoise.GetValue(pos.X *2 , pos.Y * 2, pos.Z * 2);
            if (block == 1 && pos.Y < 40 && pos.Y > 20 && coalNoise > 0.77f)
                block = 20;

            var ironNoise = _perlinNoise.GetValue(pos.X * 3, pos.Y*3, pos.Z * 3);
            if (block == 1 && pos.Y < 30 && pos.Y > 10 && ironNoise > 0.77f)
                block = 21;

            var goldNoise = _perlinNoise.GetValue(pos.X * 5, pos.Y * 5, pos.Z * 5);
            if (block == 1 && pos.Y < 20 && pos.Y > 3 && goldNoise > 0.95f)
                block = 22;

            return block;
        }

        public static ushort GenerateCaves(ushort block, Int3 pos)
        {

            return block;
        }
    }
}