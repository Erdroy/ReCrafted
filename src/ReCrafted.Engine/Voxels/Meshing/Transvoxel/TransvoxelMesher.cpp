// ReCrafted (c) 2016-2018 Always Too Late

#include "TransvoxelMesher.h"
#include "TransvoxelTables.hpp"

#include "Graphics/Mesh.h"


void TransvoxelMesher::Generate(const Vector3& position, int lod, uint8_t borders, sbyte* data)
{
    // ReSharper disable once CppUnreachableCode
    cvar voxelScale = static_cast<float>(lod);

    for(var x = VoxelChunkData::ChunkDataStart; x < VoxelChunkData::ChunkDataLength; x ++)
    {
        for (var y = VoxelChunkData::ChunkDataStart; y < VoxelChunkData::ChunkDataLength; y++)
        {
            for (var z = VoxelChunkData::ChunkDataStart; z < VoxelChunkData::ChunkDataLength; z++)
            {
                var offset = Int3(x, y, z);
                
            }
        }
    }
}

void TransvoxelMesher::Apply(const RefPtr<Mesh>& mesh)
{
    // ReSharper disable CppUnreachableCode
    mesh->SetVertices(m_vertices.Data(), m_vertices.Count());
    mesh->SetNormals(m_normals.Data());

    if(EnableUVChannelGeneration)
    {
        mesh->SetUVs(m_uvs.Data());
    }

    if(EnableColorChannelGeneration)
    {
        mesh->SetColors(m_colors.Data());
    }

    if (EnableMaterialChannelGeneration)
    {
        //mesh->SetMaterials(m_materials.Data());
    }

    mesh->SetIndices(m_indices.Data(), m_vertices.Count());

    mesh->ApplyChanges();

    Clear();
    // ReSharper restore CppUnreachableCode
}

void TransvoxelMesher::Clear()
{
    // cleanup all arrays
    m_vertices.Clear();
    m_normals.Clear();
    m_uvs.Clear();
    m_colors.Clear();
    m_materials.Clear();
    m_indices.Clear();

    // Clean vertex reuse array
    for (var i = 0u; i < m_vertexReuse.Count(); i++)
        m_vertexReuse[i] = -1;
}
