// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef IVOXELMESHER_H
#define IVOXELMESHER_H

// includes
#include "ReCrafted.h"
#include "Core/Defines.h"
#include "Voxels/SpaceObjectChunk.h"

class Mesh;

/**
 * \brief The voxel mesher interface class.
 */
struct IVoxelMesher
{
protected:
    IPhysicsShapeCooker* m_shapeCooker = nullptr;

public:
    /**
     * \brief Initializes this mesher
     * \param shapeCooker The hape cooker that will be used by this mesher.
     */
    void Initialize(IPhysicsShapeCooker* shapeCooker)
    {
        m_shapeCooker = shapeCooker;
    }

public:
    /**
    * \brief Virtual method for generating a mesh from hermite voxel data.
    * \param position The position of current procesed chunk.
    * \param lod The current procesed chunk lod.
    * \param borders Border set for current procesed chunk.
    * \param data The hermite voxel data (in -127 to 127 range).
    */
    virtual void Generate(const Vector3& position, int lod, uint8_t borders, Voxel* data) = 0;

    /**
     * \brief Checks if there are any generated triangles.
     * \return True when there is at least one triangle.
     */
    virtual bool HasTriangles() = 0;

    /**
     * \brief Uploads all data to a mesh. This also clears the mesher and prepares to next mesh generation.
     * \param chunkMesh The mesh that will get the new mesh data.
     * \param chunkCollision The collision that will get the new collision data.
     */
    virtual void Apply(const RefPtr<VoxelChunkMesh>& chunkMesh, RefPtr<VoxelChunkCollsion>& chunkCollision) = 0;

    /**
     * \brief Cleans all data used during Generate and Apply functions.
     */
    virtual void Clear() = 0;

public:
    /**
     * \brief Gets the name of mesher algorithm.  
     */
    virtual const char* GetName() = 0;
};

#endif // IMESHER_H
