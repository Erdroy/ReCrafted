// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "VoxelObjectDataAsset.h"
#include "Rendering/Heightmap.h"

void VoxelObjectDataAsset::OnInitialize()
{
    m_heightmap = new Heightmap();
}

void VoxelObjectDataAsset::OnLoadBegin(const std::string& fileName)
{
}

void VoxelObjectDataAsset::OnDeserializeBinary(uint16_t version, BinaryStream& stream)
{
    //m_heightmap->InitializeFromMemory();
}

void VoxelObjectDataAsset::OnUnload()
{
    delete m_heightmap;
}
