// ReCrafted (c) 2016-2018 Always Too Late

#include "TransvoxelMesher.h"
#include "TransvoxelTables.hpp"

#include "Graphics/Mesh.h"

TransvoxelMesher::MeshSection* TransvoxelMesher::FindSection(const VoxelMaterial_t material)
{
    for (rvar section : m_meshSections)
    {
        if (section.HasMaterial(material))
            return &section;
    }

    return nullptr;
}

TransvoxelMesher::MeshSection* TransvoxelMesher::FindSection(
    const VoxelMaterial_t materialA,
    const VoxelMaterial_t materialB,
    const VoxelMaterial_t materialC)
{
    for (rvar section : m_meshSections)
    {
        if (section.HasMaterials(materialA, materialB, materialC))
            return &section;
    }

    return nullptr;
}

TransvoxelMesher::MeshSection* TransvoxelMesher::PushSection(
    const VoxelMaterial_t materialA,
    const VoxelMaterial_t materialB, 
    const VoxelMaterial_t materialC)
{
    for (var i = m_currentSection; i < MaxSections; i++)
    {
        rvar section = m_meshSections[i];

        if (section.CanFit(materialA, materialB, materialC))
        {
            section.AddMaterial(materialA);
            section.AddMaterial(materialB);
            section.AddMaterial(materialC);
            return &section;
        }
    }

    var section = CreateSection();
    section->AddMaterial(materialA);
    section->AddMaterial(materialB);
    section->AddMaterial(materialC);

    return section;
}

TransvoxelMesher::MeshSection* TransvoxelMesher::FindOrPushSection(
    const VoxelMaterial_t materialA,
    const VoxelMaterial_t materialB,
    const VoxelMaterial_t materialC)
{
    var meshSection = FindSection(materialA, materialB, materialC);

    if (!meshSection)
        meshSection = PushSection(materialA, materialB, materialC);

    return meshSection;
}

TransvoxelMesher::MeshSection* TransvoxelMesher::CreateSection()
{
    m_currentSection++;
    ASSERT(m_currentSection < MaxSections); // TODO: Handle error
    return &GetSection();
}

void TransvoxelMesher::AddTriangle(const VertexInfo& vertexInfoA, const VertexInfo& vertexInfoB, const VertexInfo& vertexInfoC, const bool normalCorrection)
{
    var meshSection = FindOrPushSection(vertexInfoA.voxelMaterial, vertexInfoB.voxelMaterial, vertexInfoC.voxelMaterial);

    ASSERT(meshSection != nullptr);

    // Add data to the section
    meshSection->AddVertex(vertexInfoA, normalCorrection);
    meshSection->AddVertex(vertexInfoB, normalCorrection);
    meshSection->AddVertex(vertexInfoC, normalCorrection);
}

void TransvoxelMesher::PolygonizeRegularCell(const Vector3& position, Voxel* data, const float voxelScale, const int lod, const Int3& voxelOffset, const bool normalCorrection)
{
    Voxel corners[8];
    for(var i = 0; i < 8; i ++)
    {
        corners[i] = GetVoxel(data, voxelOffset + CellCorner[i]);
    }

    const byte caseCode =
          (-corners[0].value >> 7 & 0x01)
        | (-corners[1].value >> 6 & 0x02)
        | (-corners[2].value >> 5 & 0x04)
        | (-corners[3].value >> 4 & 0x08)
        | (-corners[4].value >> 3 & 0x10)
        | (-corners[5].value >> 2 & 0x20)
        | (-corners[6].value >> 1 & 0x40)
        | (-corners[7].value & 0x80);

    if((caseCode ^ ((-corners[7].value >> 7) & 0xFF)) == 0)
        return;

    cvar cellClass = RegularCellClass[caseCode];
    cvar cellData = RegularCellData[cellClass];

    uint indices[12] = {};

    cvar vertexCount = cellData.GetVertexCount();
    for(var i = 0; i < vertexCount; i ++)
    {
        cvar edgeCode = RegularVertexData[caseCode][i];

        cvar v0 = edgeCode & 0x0F;
        cvar v1 = (edgeCode & 0xF0) >> 4;

        cvar vertexId = CalculateEdgeVertexId(voxelOffset, edgeCode >> 8);
        DEBUG_ASSERT(vertexId >= 0);

        // Skip vertex calculation if already calculated
        if(m_vertexInfoMap[vertexId])
        {
            indices[i] = vertexId;
            continue;
        }

        // Calculate vertex
        cvar voxelA = corners[v0];
        cvar voxelB = corners[v1];

        cvar intersectionPosition = GetIntersection(voxelOffset + CellCorner[v0], voxelOffset + CellCorner[v1], voxelA.value, voxelB.value) * voxelScale;
        cvar vertexPosition = position + intersectionPosition;
        cvar voxelMaterial = GetVoxelMaterial(voxelA, voxelB);
        
        cvar vertexInfo = VertexInfo(vertexId, vertexPosition, voxelMaterial);
        m_vertexInfo[vertexId] = vertexInfo;
        indices[i] = vertexId;
        m_vertexInfoMap.set(vertexId);
    }

    cvar indexCount = cellData.GetTriangleCount() * 3;
    for (var i = 0; i < indexCount; i += 3)
    {
        cvar vertexId0 = indices[cellData.vertexIndex[i + 0]];
        cvar vertexId1 = indices[cellData.vertexIndex[i + 1]];
        cvar vertexId2 = indices[cellData.vertexIndex[i + 2]];

        rvar vertexInfoA = m_vertexInfo[vertexId0];
        rvar vertexInfoB = m_vertexInfo[vertexId1];
        rvar vertexInfoC = m_vertexInfo[vertexId2];

        cvar vertexPosition0 = vertexInfoA.vertexPosition;
        cvar vertexPosition1 = vertexInfoB.vertexPosition;
        cvar vertexPosition2 = vertexInfoC.vertexPosition;

        if (Vector3::Dot(vertexPosition0, vertexPosition1) < Math::Epsilon ||
            Vector3::Dot(vertexPosition1, vertexPosition2) < Math::Epsilon ||
            Vector3::Dot(vertexPosition2, vertexPosition0) < Math::Epsilon)
            continue; // zero-space triangle

        cvar normal = Vector3::Cross(vertexPosition1 - vertexPosition0, vertexPosition2 - vertexPosition0);

        // Calculate normals
        vertexInfoA.vertexNormal += normal;
        vertexInfoB.vertexNormal += normal;
        vertexInfoC.vertexNormal += normal;

        vertexInfoA.normalUses += 1;
        vertexInfoB.normalUses += 1;
        vertexInfoC.normalUses += 1;

        // Add triangle
        AddTriangle(vertexInfoA, vertexInfoB, vertexInfoC, normalCorrection);

        // Increment triangle count
        m_triangleCount++;
    }
}

void TransvoxelMesher::Generate(const Vector3& position, int lod, uint8_t borders, Voxel* data)
{
    // ReSharper disable once CppUnreachableCode
    cvar voxelScale = static_cast<float>(lod);

    for(var x = VoxelChunkData::ChunkDataStart; x < VoxelChunkData::ChunkDataLength - 1; x ++)
    {
        for (var y = VoxelChunkData::ChunkDataStart; y < VoxelChunkData::ChunkDataLength - 1; y++)
        {
            for (var z = VoxelChunkData::ChunkDataStart; z < VoxelChunkData::ChunkDataLength - 1; z++)
            {
                cvar voxelOffset = Int3(x, y, z);

                cvar normalCorrection = x < 0 || y < 0 || z < 0 || x >= 16 || y >= 16 || z >= 16;

                PolygonizeRegularCell(position, data, voxelScale, lod, voxelOffset, normalCorrection);

                // TODO: Transition cell implementation
            }
        }
    }

    // Apply normals
    for(rvar section : m_meshSections)
    {
        // Normalize the normals to be a normal normals.
        var j = 0;
        for (rvar normal : section.normals)
        {
            cvar vertexId = section.vertexIds[j];
            cvar vertexInfo = m_vertexInfo[vertexId];

            normal = vertexInfo.vertexNormal / static_cast<float>(vertexInfo.normalUses);
            normal.Normalize();

            j++;
        }
    }
}

void TransvoxelMesher::Apply(const RefPtr<VoxelChunkMesh>& chunkMesh)
{
    // Create section mesh
    cvar mesh = Mesh::CreateMesh();

    var chunkSection = VoxelChunkMesh::MeshSection();
    chunkSection.mesh = mesh;

    var meshSection = GetSection();
    for(var material : meshSection.materialSet)
        chunkSection.materialSet.Add(material);

    if (meshSection.indices.Count() == 0)
        return;

    mesh->SetVertices(meshSection.vertices.Data(), meshSection.vertices.Count());
    mesh->SetNormals(meshSection.normals.Data());
    mesh->AddCustomData(meshSection.materialsA.Data(), sizeof(Vector4));
    mesh->AddCustomData(meshSection.materialsB.Data(), sizeof(Vector4));
    mesh->AddCustomData(meshSection.materialsC.Data(), sizeof(Vector4));
    mesh->AddCustomData(meshSection.materialsD.Data(), sizeof(Vector4));
    mesh->SetIndices(meshSection.indices.Data(), meshSection.indices.Count());
    mesh->ApplyChanges();

    // Add mesh section
    chunkMesh->AddSection(chunkSection);

    Clear();
}

void TransvoxelMesher::Clear()
{
    m_triangleCount = 0u;

    // Clear only used sections
    for (var i = 0; i <= m_currentSection; i ++)
        m_meshSections[i].Clear();

    // Reset vertex info
    m_vertexInfo.Clear();
    m_vertexInfoMap.reset();

    m_currentSection = 0;
}
