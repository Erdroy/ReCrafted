// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Content/Assets/JsonAsset.h"

class VoxelObjectAsset : public JsonAsset
{
public:
    enum class DataType
    {
        CubeHeightMap,
        PreGen
    };

private:
    std::string m_name;
    std::string m_dataFile;
    DataType m_dataFileType = DataType::CubeHeightMap;
    uint32_t m_minSurfaceHeight = 0u;
    uint32_t m_maxSurfaceHeight = 0u;
    uint32_t m_hillHeight = 0u;
    uint32_t m_initialOctreeDepth = 0u;

public:
    void OnDeserializeJson(uint16_t version, const json& json) override;
    void OnLoadEnd() override {}
    void OnUnload() override {}

protected:    
    AssetType GetAssetType() override
    {
        return AssetType::VoxelObject;
    }

public:
    /// <summary>
    ///     Name of this VoxelObject.
    /// </summary>
    const std::string& ObjectName() const
    {
        return m_name;
    }

    /// <summary>
    ///     The VoxelObject data file name.
    /// </summary>
    const std::string& DataFile() const
    {
        return m_dataFile;
    }

    /// <summary>
    ///     The VoxelObject data file type.
    /// </summary>
    DataType DataFileType() const
    {
        return m_dataFileType;
    }

    /// <summary>
    /// The minimal surface height.
    /// </summary>
    uint32_t MinimumSurfaceHeight() const
    {
        return m_minSurfaceHeight;
    }

    /// <summary>
    ///     The maximal surface height. This is also the maximal build height.
    /// </summary>
    uint32_t MaximumSurfaceHeight() const
    {
        return m_maxSurfaceHeight;
    }

    /// <summary>
    ///     The maximal height of hills.
    /// </summary>
    uint32_t HillsHeight() const
    {
        return m_hillHeight;
    }

    /// <summary>
    ///     The initial depth of the VoxelObject's octree.
    /// </summary>
    uint32_t InitialOctreeDepth() const
    {
        return m_initialOctreeDepth;
    }
};
