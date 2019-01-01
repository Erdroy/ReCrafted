// ReCrafted (c) 2016-2019 Always Too Late

#include "TransvoxelMesher.h"

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
    if(m_meshSections.Size() == 0)
        return nullptr;

    if (m_meshSections.Size() == 1)
    {
        if (m_meshSections[0].HasMaterials(materialA, materialB, materialC))
            return &m_meshSections[0];
    }


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
    DEBUG_ASSERT(m_currentSection < MaxSections); // TODO: Handle error
    return &GetSection();
}

void TransvoxelMesher::AddTriangle(const VertexInfo& vertexInfoA, const VertexInfo& vertexInfoB, const VertexInfo& vertexInfoC, const bool normalCorrection)
{
    var meshSection = FindOrPushSection(vertexInfoA.voxelMaterial, vertexInfoB.voxelMaterial, vertexInfoC.voxelMaterial);

    DEBUG_ASSERT(meshSection != nullptr);

    // Add data to the section
    meshSection->AddVertex(vertexInfoA, normalCorrection);
    meshSection->AddVertex(vertexInfoB, normalCorrection);
    meshSection->AddVertex(vertexInfoC, normalCorrection);
}
