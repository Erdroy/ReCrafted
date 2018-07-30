// ReCrafted (c) 2016-2018 Always Too Late

#include "Voxel.h"

const sbyte Voxel::FullValue = -127;
const sbyte Voxel::EmptyValue = 127;

const Voxel Voxel::Default = Voxel{ static_cast<sbyte>(0), 0u };
const Voxel Voxel::Empty = Voxel{ EmptyValue, 0u };
const Voxel Voxel::Full = Voxel{ FullValue, 0u };
