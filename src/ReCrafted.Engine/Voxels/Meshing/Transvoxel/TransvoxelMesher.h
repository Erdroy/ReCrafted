// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef TRANSVOXELMESHER_H
#define TRANSVOXELMESHER_H

// includes
#include "../IVoxelMesher.h"

class TransvoxelMesher : IVoxelMesher
{
public:
    TransvoxelMesher() = default;

    virtual ~TransvoxelMesher() = default;

public:
    /**
    * \brief Virtual method for generating a mesh from hermite voxel data.
    * \param data The hermite voxel data (in -127 to 127 range).
    */
    void Generate(const Vector3& position, int lod, uint8_t borders, sbyte* data) override;

    /**
    * \brief Checks if there are any generated triangles.
    * \return True when there is at least one triangle.
    */
    bool HasTriangles() override
    {
        return false;
    }

    /**
    * \brief Uploads all data to a mesh. This also clears the mesher and prepares to next mesh generation.
    * \param mesh The mesh that will get the new mesh data.
    */
    void Apply(const RefPtr<Mesh>& mesh) override;

    /**
    * \brief Cleans all data used during Generate and Apply functions.
    */
    void Clean() override;
};

#endif // TRANSVOXELMESHER_H
