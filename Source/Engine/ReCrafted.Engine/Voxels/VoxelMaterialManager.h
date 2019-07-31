// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Common/Lock.h"
#include "Core/SubSystems/SubSystem.h"
#include "Voxels/Voxel.h"
#include "Voxels/Assets/VoxelMaterial.h"

#include <sparsepp/spp.h>

API_USING("ReCrafted.API.Content");
API_USING("ReCrafted.API.Rendering");

API_CUSTOM_TRANSLATION(VoxelMaterial_t, "ushort")

API_CLASS(public, static, noinherit)
class VoxelMaterialManager : public SubSystem<VoxelMaterialManager>
{
    API_CLASS_BODY();
    friend class VoxelMaterial;

private:
    spp::sparse_hash_map<VoxelMaterial_t, VoxelMaterial*> m_materials;
    std::vector<VoxelMaterial*> m_allMaterials;
    Lock m_materialLock;

    VoxelMaterial* m_defaultMaterial = nullptr;

public:
    virtual ~VoxelMaterialManager() = default;

protected:
    void Initialize() override;
    void Shutdown() override;

private:
    void RegisterMaterial(VoxelMaterial* voxelMaterial);
    void UnregisterMaterial(VoxelMaterial* voxelMaterial);

public:
    /// <summary>
    ///     Adds material to the registry.
    /// </summary>
    API_FUNCTION()
    static void AddMaterial(VoxelMaterial* material);

    /// <summary>
    ///     Sets the default material which is being applied, when 
    ///     some voxel uses material id that does not exist.
    /// </summary>
    API_FUNCTION()
    static void SetDefaultMaterial(VoxelMaterial* material);

    /// <summary>
    ///     Gets material by it's id.
    /// </summary>
    API_FUNCTION()
    static VoxelMaterial* GetMaterial(VoxelMaterial_t id);
};
