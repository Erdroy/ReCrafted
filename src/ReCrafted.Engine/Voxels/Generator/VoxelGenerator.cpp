// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "VoxelGenerator.h"
#include "Core/Math/FastNoiseSIMD/FastNoiseSIMD.h"

FastNoiseSIMD* noise_terrain = nullptr;
float* noise_terrain_set = nullptr;

void VoxelGenerator::initialize()
{
	// https://github.com/Auburns/FastNoiseSIMD/wiki

	FastNoiseSIMD::SetSIMDLevel(-1);
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
	noise_terrain_set = noise_terrain->GetSimplexFractalSet(x, 0, z, 16, 1, 16);
}

void VoxelGenerator::endChunk()
{
	if (noise_terrain)
		noise_terrain->FreeNoiseSet(noise_terrain_set);
}

voxelid VoxelGenerator::generate(int index, int y)
{
	auto noise = noise_terrain_set[index];

	auto hillLevel = (1.0f - noise) * 10.0;
	auto baseLevel = 10.0f + hillLevel;

	if(float(y) < baseLevel)
		return 1u;

	if (float(y) - 2 < baseLevel)
		return 2u;

	return voxel_air;
}
