// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Common/Array.h"
#include "Common/List.h"
#include "Scripting/Object.h"
#include "Rendering/Materials/Material.h"

API_USING("ReCrafted.API.Content")
API_USING("ReCrafted.API.Mathematics")

/// <summary>
///     Model asset class.
/// </summary>
API_CLASS(public, sealed, partial, customNamespace = "ReCrafted.API.Rendering")
class Model final : public BinaryAsset
{
    API_CLASS_BODY()

private:
    List<Mesh*> m_meshes = {};
    List<Material*> m_materials = {};

protected:
    void OnDeserializeBinary(uint16_t version, BinaryStream& stream) override;
    void OnLoadEnd() override;
    void OnUnload() override;

    AssetType GetAssetType() override
    {
        return AssetType::Model;
    }

    bool CanLoadAsync() override
    {
        return true;
    }

public:
    /// <summary>
    ///     Gets all meshes rendered by this model.
    /// </summary>
    Array<Mesh*> GetMeshes()
    {
        return Array<Mesh*> { m_meshes.Data(), m_meshes.Count() };
    }

    /// <summary>
    ///     Gets all of the bound materials from this model.
    /// </summary>
    API_PROPERTY(noprefix)
    Array<Material*> GetMaterials()
    {
        return Array<Material*> { m_materials.Data(), m_materials.Count() };
    }
};