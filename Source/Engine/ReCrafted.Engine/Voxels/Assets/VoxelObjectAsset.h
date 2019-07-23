// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Content/Assets/JsonAsset.h"
#include "Voxels/VoxelObjectBase.h"

API_USING("ReCrafted.API.Content");

enum class VoxelObjectDataType
{
    Unknown,

    CubeHeightMap,
    PreGen,

    Count
};

enum class VoxelObjectType
{
    Unknown,

    Planet,
    Asteroid,

    Count
};

API_CLASS(public, sealed, customNamespace="ReCrafted.API.Voxels")
class VoxelObjectAsset : public JsonAsset
{
    API_CLASS_BODY();

private:
    std::string m_name;
    std::string m_dataFile;
    VoxelObjectDataType m_dataFileType = VoxelObjectDataType::Unknown;
    VoxelObjectType m_type = VoxelObjectType::Unknown;
    uint32_t m_minSurfaceHeight = 0u;
    uint32_t m_maxSurfaceHeight = 0u;
    uint32_t m_hillHeight = 0u;
    uint32_t m_initialOctreeDepth = 0u;
    VoxelObjectBase* m_voxelObject = nullptr;

private:
    void LoadDescription(const json& json);

public:
    void OnDeserializeJson(uint16_t version, const json& json) override;
    void OnLoadEnd() override;
    void OnUnload() override;

protected:    
    AssetType GetAssetType() override
    {
        return AssetType::VoxelObject;
    }

public:
    /// <summary>
    ///     Name of this VoxelObject.
    /// </summary>
    API_PROPERTY()
    const char* ObjectName() const
    {
        return m_name.c_str();
    }

    /// <summary>
    ///     The VoxelObject data file name.
    /// </summary>
    API_PROPERTY()
    const char* DataFile() const
    {
        return m_dataFile.c_str();
    }

    /// <summary>
    ///     The VoxelObject data file type.
    /// </summary>
    API_PROPERTY()
    VoxelObjectDataType DataFileType() const
    {
        return m_dataFileType;
    }

    /// <summary>
    ///     The VoxelObject type.
    /// </summary>
    API_PROPERTY()
    VoxelObjectType ObjectType() const
    {
        return m_type;
    }

    /// <summary>
    /// The minimal surface height.
    /// </summary>
    API_PROPERTY()
    uint32_t MinimumSurfaceHeight() const
    {
        return m_minSurfaceHeight;
    }

    /// <summary>
    ///     The maximal surface height. This is also the maximal build height.
    /// </summary>
    API_PROPERTY()
    uint32_t MaximumSurfaceHeight() const
    {
        return m_maxSurfaceHeight;
    }

    /// <summary>
    ///     The maximal height of hills.
    /// </summary>
    API_PROPERTY()
    uint32_t HillsHeight() const
    {
        return m_hillHeight;
    }

    /// <summary>
    ///     The initial depth of the VoxelObject's octree.
    /// </summary>
    API_PROPERTY()
    uint32_t InitialOctreeDepth() const
    {
        return m_initialOctreeDepth;
    }

    /// <summary>
    ///     Gets the voxel object from this asset.
    /// </summary>
    API_PROPERTY()
    VoxelObjectBase* VoxelObject() const
    {
        return m_voxelObject;
    }
};
