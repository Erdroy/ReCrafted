// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "VoxelGenerator.h"
#include "../../../Core/Math/FastNoiseSIMD/FastNoiseSIMD.h"

FastNoiseSIMD* noise_terrain = nullptr;
float* noise_terrain_set = nullptr;

void VoxelGenerator::initialize()
{
	// https://github.com/Auburns/FastNoiseSIMD/wiki

	FastNoiseSIMD::SetSIMDLevel(2);
	noise_terrain = FastNoiseSIMD::NewFastNoiseSIMD(100);
	noise_terrain->SetNoiseType(FastNoiseSIMD::SimplexFractal);
	noise_terrain->SetFrequency(0.008f);
	noise_terrain->SetFractalType(FastNoiseSIMD::FBM);
	noise_terrain->SetFractalOctaves(4);
	noise_terrain->SetFractalLacunarity(2.0f);
	noise_terrain->SetFractalGain(0.5f);

}

void VoxelGenerator::beginChunk(int x, int z)
{
	if(noise_terrain)
		noise_terrain->FreeNoiseSet(noise_terrain_set);

	noise_terrain_set = noise_terrain->GetSimplexFractalSet(x, 0, z, 16, 1, 16);
}

voxelid VoxelGenerator::generate(int x, int y, int z)
{
	auto noise = noise_terrain_set[z * ChunkWidth + x];

	auto hillLevel = (1.0f - noise) * 10.0;
	auto baseLevel = 10.0f + hillLevel;

	if(float(y) > baseLevel)
		return 0u;

	return 1u;
}
