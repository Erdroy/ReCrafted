// ReCrafted (c) 2016-2018 Always Too Late

#include "Voxel.h"

const VoxelValue_t Voxel::FullValue = -127;
const VoxelValue_t Voxel::EmptyValue = 127;

const Voxel Voxel::Default = Voxel{ static_cast<VoxelValue_t>(0), 0u };
const Voxel Voxel::Empty = Voxel{ EmptyValue, 0u };
const Voxel Voxel::Full = Voxel{ FullValue, 0u };
