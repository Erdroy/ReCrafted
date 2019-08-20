// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

class Mesh;
class VoxelChunkMesh;
class VoxelChunkCollision;

/// <summary>
///     The voxel mesher interface class.
/// </summary>
struct IVoxelMesher
{
    DELETE_COPY_MOVE(IVoxelMesher)

protected:
    ShapeCooker* m_shapeCooker = nullptr;

public:
    IVoxelMesher() = default;
    virtual ~IVoxelMesher() = default;

public:
    /// <summary>
    ///     Initializes this mesher
    /// </summary>
    /// <param name="shapeCooker">The shape cooker that will be used by this mesher.</param>
    void Initialize(ShapeCooker* shapeCooker)
    {
        m_shapeCooker = shapeCooker;
    }

public:
    /// <summary>
    ///     Virtual method for generating a mesh from hermite voxel data.
    /// </summary>
    /// <param name="position">The position of current processed chunk.</param>
    /// <param name="lod">The current processed chunk lod.</param>
    /// <param name="borders">Border set for current processed chunk.</param>
    /// <param name="data">The hermite voxel data (in -127 to 127 range).</param>
    virtual void Generate(const Vector3& position, int lod, uint8_t borders, Voxel* data) = 0;

    /// <summary>
    ///     Checks if there are any generated triangles.
    /// </summary>
    /// <returns>True when there is at least one triangle.</returns>
    virtual bool HasTriangles() = 0;

    /// <summary>
    ///     Uploads all data to a mesh. This also clears the mesher and prepares to next mesh generation.
    /// </summary>
    /// <param name="chunkMesh">The mesh that will get the new mesh data.</param>
    /// <param name="chunkCollision">The collision that will get the new collision data.</param>
    virtual void Apply(const RefPtr<VoxelChunkMesh>& chunkMesh, MeshCollider* chunkCollision) = 0;

    /// <summary>
    ///     Cleans all data used during Generate and Apply functions.
    /// </summary>
    virtual void Clear() = 0;

public:
    /// <summary>
    ///     Gets the name of mesher algorithm.  
    /// </summary>
    virtual const char* GetName() = 0;
};
