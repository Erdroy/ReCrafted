// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "VoxelPlanet.h"

VoxelPlanet::VoxelPlanet()
{
    // TODO: Register
}

VoxelPlanet::~VoxelPlanet()
{
    // TODO: Unregister
}

void VoxelPlanet::Initialize()
{
    // Temporary initialization
    m_position = Vector3d::Zero;
    m_bounds = BoundingSphereD(m_position, 1024.0);
}