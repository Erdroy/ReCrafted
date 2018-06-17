// ReCrafted (c) 2016-2018 Always Too Late

#include "DebugDraw.h"
#include "Graphics.h"
#include "Camera.h"
#include "Core/Streams/FileStream.h"

SINGLETON_IMPL(DebugDraw)

namespace DebugMesh
{
    Vector3 m_wireCubeEdges[12][2] = {
        {
            Vector3(-1.0f, -1.0f, 1.0f),
            Vector3(-1.0f, -1.0f, -1.0f),
        },
        {
            Vector3(1.0f, -1.0f, 1.0f),
            Vector3(1.0f, -1.0f, -1.0f),
        },
        {
            Vector3(1.0f, -1.0f, 1.0f),
            Vector3(-1.0f, -1.0f, 1.0f),
        },
        {
            Vector3(1.0f, -1.0f, -1.0f),
            Vector3(-1.0f, -1.0f, -1.0f),
        },

        {
            Vector3(-1.0f, 1.0f, 1.0f),
            Vector3(-1.0f, 1.0f, -1.0f),
        },
        {
            Vector3(1.0f, 1.0f, 1.0f),
            Vector3(1.0f, 1.0f, -1.0f),
        },
        {
            Vector3(1.0f, 1.0f, 1.0f),
            Vector3(-1.0f, 1.0f, 1.0f),
        },
        {
            Vector3(1.0f, 1.0f, -1.0f),
            Vector3(-1.0f, 1.0f, -1.0f),
        },

        {
            Vector3(-1.0f, -1.0f, 1.0f),
            Vector3(-1.0f, 1.0f, 1.0f),
        },
        {
            Vector3(1.0f, -1.0f, 1.0f),
            Vector3(1.0f, 1.0f, 1.0f),
        },
        {
            Vector3(1.0f, -1.0f, -1.0f),
            Vector3(1.0f, 1.0f, -1.0f),
        },
        {
            Vector3(-1.0f, -1.0f, -1.0f),
            Vector3(-1.0f, 1.0f, -1.0f),
        }
    };

    Vector3 m_cubeVertices[8] = {
        Vector3(-1.0f, 1.0f, -1.0f),    // 0  b---
        Vector3(-1.0f, 1.0f,  1.0f),    // 1  f---
        Vector3(1.0f, 1.0f,  1.0f),     // 2  ---f
        Vector3(1.0f, 1.0f, -1.0f),     // 3  ---b

        Vector3(-1.0f, -1.0f, -1.0f),   // 4  f---
        Vector3(-1.0f, -1.0f,  1.0f),   // 5  b---
        Vector3(1.0f, -1.0f,  1.0f),    // 6  ---f
        Vector3(1.0f, -1.0f, -1.0f),    // 7  ---b
    };

    uint16_t m_cubeIndices[6] = {
        0, 1, 2, 2, 3, 0, // top
        /*6, 5, 4, 4, 7, 6, // bottom

        7, 4, 0, 0, 3, 7, // front
        1, 5, 6, 6, 2, 1, // back

        1, 0, 5, 0, 4, 5, // left
        6, 7, 2, 7, 3, 2  // right*/
    };
}

void DebugDraw::OnInit()
{
    m_debugShader = Renderer::CreateShader("../assets/shaders/Debug.shader");

    m_linesVB = Renderer::CreateVertexBuffer(Batch::maxPointsPerBatch, sizeof(Point), true);
    m_trianglesVB = Renderer::CreateVertexBuffer(Batch::maxVerticesPerBatch, sizeof(Vertex), true);
    m_trianglesIB = Renderer::CreateIndexBuffer(Batch::maxVerticesPerBatch, nullptr, true, true);
}

void DebugDraw::OnLoad()
{
}

void DebugDraw::Update()
{
}

void DebugDraw::RenderLines(Batch& batch)
{
    rvar lineList = batch.GetLineList();
    cvar pointCount = lineList.Count();

    if (pointCount == 0)
        return;

    // Set line draw mode
    Graphics::GetInstance()->SetStage(RenderStage::DebugDrawLines);

    // Allocate vertex buffer
    cvar dataSize = pointCount * sizeof(Point);
    cvar data = Renderer::Allocate(dataSize);

    // Copy line list into data
    memcpy_s(data, dataSize, lineList.Data(), dataSize);

    // Update vertex buffer
    Renderer::UpdateVertexBuffer(m_linesVB, data, static_cast<uint>(dataSize), 0u);

    // Apply vertex buffer
    Renderer::ApplyVertexBuffer(m_linesVB);

    // Draw
    Renderer::Draw(pointCount);
}

void DebugDraw::RenderTriangles(Batch& batch)
{
    rvar vertexList = batch.GetVertexList();
    rvar indexList = batch.GetIndexList();
    cvar vertexCount = vertexList.Count();
    cvar indexCount = indexList.Count();

    if (vertexCount == 0)
        return;

    // Set triangle draw mode
    Graphics::GetInstance()->SetStage(RenderStage::DebugDrawTriangles);

    // Allocate vertex buffer
    cvar vertexDataSize = vertexCount * sizeof(Vertex);
    cvar vertexData = Renderer::Allocate(vertexDataSize);

    // Copy vertex list into data
    memcpy_s(vertexData, vertexDataSize, vertexList.Data(), vertexDataSize);

    // Allocate index buffer
    cvar indexDataSize = indexCount * sizeof(uint);
    cvar indexData = Renderer::Allocate(indexDataSize);

    // Copy vertex list into data
    memcpy_s(indexData, indexDataSize, indexList.Data(), indexDataSize);

    // Update vertex buffer and index buffer
    Renderer::UpdateVertexBuffer(m_trianglesVB, vertexData, static_cast<uint>(vertexDataSize), 0u);
    Renderer::UpdateIndexBuffer(m_trianglesIB, vertexData, static_cast<uint>(indexDataSize), 0u);

    // Apply vertex buffer
    Renderer::ApplyVertexBuffer(m_trianglesVB);
    Renderer::ApplyIndexBuffer(m_trianglesIB);

    // Draw
    Renderer::Draw(indexCount);
}

void DebugDraw::Render()
{
    // Set shader values
    rvar mvpMatrix = Camera::GetMainCamera()->GetViewProjection();
    Renderer::SetShaderValue(m_debugShader, 0, 0, &mvpMatrix, sizeof(Matrix));

    // Apply shader
    Renderer::ApplyShader(m_debugShader, 0);

    for (rvar batch : m_batches)
    {
        RenderTriangles(batch);
        RenderLines(batch);

        batch.Clear();
    }

    // We don't want to remove batches, but only reset to the first one
    m_currentBatch = &m_batches.First();
}

void DebugDraw::OnDispose()
{
    m_batches.Release();

    Renderer::DestroyShader(m_debugShader);
}

void DebugDraw::InternalDrawLine(const Vector3& start, const Vector3& end)
{
    // Add new line to the current batch
    var batch = GetBatch();
    rvar lineList = batch->GetLineList();

    lineList.Add(Point{ start, m_currentColor });
    lineList.Add(Point{ end, m_currentColor });
}

DebugDraw::Batch* DebugDraw::GetBatch()
{
    if (m_currentBatch->IsFull())
    {
        ASSERT(m_batches.Count() < m_maxBatches);

        // Check if there is next batch available
        // - if not: create new batch
        // Use new batch
        cvar idx = m_batches.IndexOf(*m_currentBatch);

        if (idx < static_cast<int>(m_batches.Size()) - 1)
        {
            // Select next batch
            m_currentBatch = &m_batches[idx + 1];
        }
        else
        {
            // Create new batch
            m_batches.Add({});

            // Set the new batch as first
            m_currentBatch = &m_batches.Last();
        }
    }

    return m_currentBatch;
}

void DebugDraw::SetColor(const Color& color)
{
    ASSERT(IS_MAIN_THREAD());

    m_instance->m_currentColor = color.ToVector4();
}

void DebugDraw::DrawLine(const Vector3& start, const Vector3& end)
{
    ASSERT(IS_MAIN_THREAD());

    m_instance->InternalDrawLine(start, end);
}

void DebugDraw::DrawBox(const Vector3& center, const Vector3& size)
{
    ASSERT(IS_MAIN_THREAD());

    cvar halfSize = size * 0.5f;

    // Add new triangles to the current batch
    var batch = m_instance->GetBatch();
    rvar vertexList = batch->GetVertexList();
    rvar indexList = batch->GetIndexList();

    cvar index = vertexList.Count();

    for (cvar vertexOffset : DebugMesh::m_cubeVertices)
    {
        cvar point = center + vertexOffset * halfSize;
        vertexList.Add({ point, m_instance->m_currentColor });
    }

    for (cvar indexOffset : DebugMesh::m_cubeIndices)
    {
        indexList.Add(index + indexOffset);
    }
}

void DebugDraw::DrawWireBox(const Vector3& center, const Vector3& size)
{
    cvar halfSize = size * 0.5f;

    for (crvar edge : DebugMesh::m_wireCubeEdges)
        m_instance->DrawLine(center + edge[0] * halfSize, center + edge[1] * halfSize);
}
