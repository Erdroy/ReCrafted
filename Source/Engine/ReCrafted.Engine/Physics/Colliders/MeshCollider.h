// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Common/Array.h"
#include "Physics/Colliders/Collider.h"

API_USING("ReCrafted.API.Rendering");
API_USING("ReCrafted.API.Mathematics");

/// <summary>
///     MeshCollider class.
/// </summary>
/// <remarks>Before attaching to an actor, use SetMesh to fully initialize this class.</remarks>
/// <remarks>MeshCollider is only valid for static and dynamic kinematic RigidBodies.</remarks>
API_CLASS(public, sealed, customNamespace = "ReCrafted.API.Physics")
class MeshCollider final : public Collider
{
    API_CLASS_BODY()
    DELETE_COPY_MOVE(MeshCollider)

public:
    MeshCollider();
    ~MeshCollider();

public:
    /// <summary>
    ///     Makes this collider to match given mesh.
    /// </summary>
    /// <remarks>To properly use mesh, please make sure that it is not applied or is applied with 'releaseMeshData' flag in ApplyChanges.</remarks>
    API_FUNCTION()
    void SetMesh(Mesh* mesh);

    /// <summary>
    ///     Makes this collider to match given mesh using specified shapeCooker.
    /// </summary>
    /// <remarks>To properly use mesh, please make sure that it is not applied or is applied with 'releaseMeshData' flag in ApplyChanges.</remarks>
    void SetMesh(Mesh* mesh, ShapeCooker* shapeCooker);

    /// <summary>
    ///     Makes this collider to match given raw/procedural mesh data.
    /// </summary>
    API_FUNCTION()
    void SetMesh(const Array<Vector3>& vertices, const Array<uint32_t>& indices);

    /// <summary>
    ///     Makes this collider to match given raw/procedural mesh data using specified shapeCooker.
    /// </summary>
    void SetMesh(const Array<Vector3>& vertices, const Array<uint32_t>& indices, ShapeCooker* shapeCooker);
};
