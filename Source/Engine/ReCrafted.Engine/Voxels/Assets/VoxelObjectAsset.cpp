// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "VoxelObjectAsset.h"
#include "Voxels/VoxelObjectBase.h"
#include "Voxels/VoxelPlanet.h"

void VoxelObjectAsset::LoadDescription(const json& json)
{
    const auto mName = json["ObjectName"];
    ASSERT(!mName.is_null());
    ASSERT(mName.is_string());
    m_name = mName.get<std::string>();

    const auto mDataFile = json["ObjectFile"];
    ASSERT(!mDataFile.is_null());
    ASSERT(mDataFile.is_string());
    m_dataFile = mDataFile.get<std::string>();

    const auto mDataFileType = json["ObjectFileType"];
    ASSERT(!mDataFileType.is_null());
    ASSERT(mDataFileType.is_number_integer());
    m_dataFileType = static_cast<VoxelObjectDataType>(mDataFileType.get<int>());

    const auto mObjectType = json["ObjectFileType"];
    ASSERT(!mObjectType.is_null());
    ASSERT(mObjectType.is_number_integer());
    m_type = static_cast<VoxelObjectType>(mObjectType.get<int>());

    const auto mMinSurf = json["MinimalSurfaceHeight"];
    ASSERT(!mMinSurf.is_null());
    ASSERT(mMinSurf.is_number_unsigned());
    m_minSurfaceHeight = static_cast<uint32_t>(mMinSurf.get<uint32_t>());

    const auto mMaxSurf = json["MaximalSurfaceHeight"];
    ASSERT(!mMaxSurf.is_null());
    ASSERT(mMaxSurf.is_number_unsigned());
    m_maxSurfaceHeight = static_cast<uint32_t>(mMaxSurf.get<uint32_t>());

    const auto mHillsHeight = json["HillsHeight"];
    ASSERT(!mHillsHeight.is_null());
    ASSERT(mHillsHeight.is_number_unsigned());
    m_hillHeight = static_cast<uint32_t>(mHillsHeight.get<uint32_t>());

    const auto mOctreeDepth = json["InitialOctreeDepth"];
    ASSERT(!mOctreeDepth.is_null());
    ASSERT(mOctreeDepth.is_number_unsigned());
    m_initialOctreeDepth = static_cast<uint32_t>(mOctreeDepth.get<uint32_t>());
}

void VoxelObjectAsset::OnDeserializeJson(uint16_t version, const json& json)
{
    LoadDescription(json);
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
        _ASSERT_(false, "Not implemented");
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
    if(m_voxelObject)
    {
        Destroy(m_voxelObject);
        m_voxelObject = nullptr;
    }
}
