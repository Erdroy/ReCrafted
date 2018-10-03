// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef VOXELMATERIALMANAGER_H
#define VOXELMATERIALMANAGER_H

// includes
#include "ReCrafted.h"
#include "Core/EngineComponent.h"
#include "Core/Lock.h"
#include "Voxel.h"

#include <spp.h>

class VoxelMaterialManager : public EngineComponent<VoxelMaterialManager>
{
    friend class EngineMain;
    friend class VoxelMaterial;

private:
    SCRIPTING_API_IMPL()

private:
    spp::sparse_hash_map<VoxelMaterial_t, VoxelMaterial*> m_materials;
    Lock m_materialLock;

public:
    virtual ~VoxelMaterialManager() = default;

private:
    void OnInit() override;
    void OnDispose() override;
    void Update() override;
    
private:
    void RegisterMaterial(VoxelMaterial* voxelMaterial);
    void UnregisterMaterial(VoxelMaterial* voxelMaterial);

public:
    static VoxelMaterial* GetMaterial(VoxelMaterial_t id);
};

#endif // VOXELMATERIALMANAGER_H
