// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "TransvoxelMesher.h"

TransvoxelMesher::MeshSection* TransvoxelMesher::FindSection(const VoxelMaterial_t material)
{
    for (auto& section : m_meshSections)
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
    if(m_meshSections.Size() == 0)
        return nullptr;

    if (m_meshSections.Size() == 1)
    {
        if (m_meshSections[0].HasMaterials(materialA, materialB, materialC))
            return &m_meshSections[0];
    }


    for (auto& section : m_meshSections)
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
    for (auto i = m_currentSection; i < MaxSections; i++)
    {
        auto& section = m_meshSections[i];

        if (section.CanFit(materialA, materialB, materialC))
        {
            section.AddMaterial(materialA);
            section.AddMaterial(materialB);
            section.AddMaterial(materialC);
            return &section;
        }
    }

    auto section = CreateSection();
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
    auto meshSection = FindSection(materialA, materialB, materialC);

    if (!meshSection)
        meshSection = PushSection(materialA, materialB, materialC);

    return meshSection;
}

TransvoxelMesher::MeshSection* TransvoxelMesher::CreateSection()
{
    m_currentSection++;
    DEBUG_ASSERT(m_currentSection < MaxSections); // TODO: Handle error
    return &GetSection();
}

void TransvoxelMesher::AddTriangle(const VertexInfo& vertexInfoA, const VertexInfo& vertexInfoB, const VertexInfo& vertexInfoC, const bool normalCorrection)
{
    auto meshSection = FindOrPushSection(vertexInfoA.voxelMaterial, vertexInfoB.voxelMaterial, vertexInfoC.voxelMaterial);

    DEBUG_ASSERT(meshSection != nullptr);

    // Add data to the section
    meshSection->AddVertex(vertexInfoA, normalCorrection);
    meshSection->AddVertex(vertexInfoB, normalCorrection);
    meshSection->AddVertex(vertexInfoC, normalCorrection);
}
