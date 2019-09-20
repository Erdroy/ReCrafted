// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Content/Assets/BinaryAsset.h"
#include "Voxels/VoxelObjectBase.h"

API_USING("ReCrafted.API.Content");

API_CLASS(public, sealed, customNamespace = "ReCrafted.API.Voxels")
class VoxelObjectDataAsset : public BinaryAsset
{
    API_CLASS_BODY();
    ASSET_BODY(VoxelObjectDataAsset);

private:
    Heightmap* m_heightmap = nullptr;

protected:
    void OnInitialize() override;
    void OnLoadBegin(const std::string& fileName) override;
    void OnDeserializeBinary(uint16_t version, BinaryStream& stream) override;
    void OnUnload() override;

    AssetType GetAssetType() override
    {
        return AssetType::VoxelObjectData;
    }

    bool CanLoadAsync() override
    {
        return true;
    }
};