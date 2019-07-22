// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "VoxelPlanet.h"

VoxelPlanet::VoxelPlanet()
{
}

VoxelPlanet::~VoxelPlanet()
{
}

void VoxelPlanet::Initialize()
{
    Load("Voxels/Objects/Moon");

    // Call base function at the end
    VoxelObjectBase::Initialize();
}