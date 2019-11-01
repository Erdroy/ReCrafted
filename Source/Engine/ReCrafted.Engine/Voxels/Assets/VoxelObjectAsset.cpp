// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "VoxelObjectAsset.h"
#include "Content/ContentManager.h"
#include "Voxels/VoxelObjectBase.h"
#include "Voxels/VoxelPlanet.h"
#include "Core/Heightmap.h"


void VoxelObjectAsset::OnDeserializeBinary(uint16_t version, BinaryStream& stream)
{
    m_name = stream.ReadString();
    m_type = static_cast<VoxelObjectType>(stream.ReadInt32());
    m_minSurfaceHeight = stream.ReadInt32();
    m_maxSurfaceHeight = stream.ReadInt32();
    m_hillsHeight = stream.ReadInt32();
    m_initialOctreeDepth = stream.ReadInt32();

    // Read heightmap
    const auto width = stream.ReadInt32();
    const auto height = stream.ReadInt32();
    const auto is16Bit = stream.ReadBool();
    const auto mips = stream.ReadInt32();
    const auto isCubical = stream.ReadBool();

    auto heightmapFormat = HeightmapFormat::Unknown;
    if (isCubical)
    {
        if (is16Bit)
            heightmapFormat = HeightmapFormat::Cubical_16bit;
        else
            heightmapFormat = HeightmapFormat::Cubical_8bit;
    }
    else
        _ASSERT_(false, "Not implemented");

    m_heightmap = new Heightmap(heightmapFormat);
    m_heightmap->InitializeFromStream(width, height, mips, &stream);
}

void VoxelObjectAsset::OnLoadEnd()
{
    // Initialize VoxelObject
    switch (m_type)
    {
    case VoxelObjectType::Planet:
        m_voxelObject = New<VoxelPlanet>();
        break;
    case VoxelObjectType::Asteroid:
        _ASSERT_(false, "Asteroid/PreGen is not implemented, yet");
        //m_voxelObject = New<VoxelAsteroid>();
        break;

    case VoxelObjectType::Unknown:
    case VoxelObjectType::Count:
    default:  break;
    }

    ASSERT(m_voxelObject);
    m_voxelObject->m_asset = this;
    m_voxelObject->Initialize();
}

void VoxelObjectAsset::OnUnload()
{
    delete m_heightmap;

    if(m_voxelObject)
    {
        Destroy(m_voxelObject);
        m_voxelObject = nullptr;
    }
}
