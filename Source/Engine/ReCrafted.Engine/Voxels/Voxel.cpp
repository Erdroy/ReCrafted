// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "Voxel.h"

const VoxelMaterial_t Voxel::EmptyMaterial = 0u;
const VoxelValue_t Voxel::FullValue = -127;
const VoxelValue_t Voxel::EmptyValue = 127;

const Voxel Voxel::Default = Voxel{ static_cast<VoxelValue_t>(0), 0u };
const Voxel Voxel::Empty = Voxel{ EmptyValue, EmptyMaterial };
const Voxel Voxel::Full = Voxel{ FullValue, 1u };
