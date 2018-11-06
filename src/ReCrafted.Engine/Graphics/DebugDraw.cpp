// ReCrafted (c) 2016-2018 Always Too Late

#include "DebugDraw.h"
#include "Graphics.h"
#include "Camera.h"
#include "Common/Profiler/Profiler.h"

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

    uint32_t m_cubeIndices[36] = {
        0, 1, 2, 2, 3, 0, // top
        6, 5, 4, 4, 7, 6, // bottom

        7, 4, 0, 0, 3, 7, // front
        1, 5, 6, 6, 2, 1, // back

        1, 0, 5, 0, 4, 5, // left
        6, 7, 2, 7, 3, 2  // right
    };
}

void DebugDraw::OnInit()
{
    m_debugShader = Renderer::CreateShader("../content/Shaders/Debug.rcasset");

    m_linesVB = Renderer::CreateVertexBuffer(Batch::maxPointsPerBatch, sizeof(Point), true);
    m_trianglesVB = Renderer::CreateVertexBuffer(Batch::maxVerticesPerBatch, sizeof(Vertex), true);
    m_trianglesIB = Renderer::CreateIndexBuffer(Batch::maxIndicesPerBatch, nullptr, true, true);

    SetMatrix(Matrix::Identity);
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
    Renderer::UpdateIndexBuffer(m_trianglesIB, indexData, static_cast<uint>(indexDataSize), 0u);

    // Apply vertex buffer
    Renderer::ApplyVertexBuffer(m_trianglesVB);
    Renderer::ApplyIndexBuffer(m_trianglesIB);

    // Draw
    Renderer::DrawIndexed(indexCount);
}

void DebugDraw::Render()
{
    Profiler::BeginProfile(__FUNCTION__);

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

    // Reset matrix
    SetMatrix(Matrix::Identity);

    Profiler::EndProfile();
}

void DebugDraw::OnDispose()
{
    m_batches.Release();

    Renderer::DestroyVertexBuffer(m_linesVB);
    Renderer::DestroyVertexBuffer(m_trianglesVB);
    Renderer::DestroyIndexBuffer(m_trianglesIB);

    Renderer::DestroyShader(m_debugShader);
}

void DebugDraw::TransformPoint(Vector3& point)
{
    if (m_matrix.IsIdentity())
        return;

    point = Vector3::Transform(point, m_matrix);
}

void DebugDraw::InternalDrawLine(const Vector3& start, const Vector3& end)
{
    // Add new line to the current batch
    var batch = GetBatch();
    rvar lineList = batch->GetLineList();

    var point1 = start;
    var point2 = end;
    
    m_instance->TransformPoint(point1);
    m_instance->TransformPoint(point2);

    lineList.Add(Point{ point1, m_currentColor });
    lineList.Add(Point{ point2, m_currentColor });
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

Color DebugDraw::GetColor()
{
    return Color::FromVector4(m_instance->m_currentColor);
}

void DebugDraw::SetMatrix(const Matrix& matrix)
{
    m_instance->m_matrix = matrix;
}

Matrix& DebugDraw::GetMatrix()
{
    return m_instance->m_matrix;
}

void DebugDraw::DrawArrow(const Vector3& start, const Vector3& end, float arrowSize)
{
    var direction = Vector3::Normalize(end - start);
    var directionToCamera = Vector3::Normalize(end - Camera::GetMainCamera()->GetPosition());
    cvar arrowOffsetAxis = Vector3::Cross(direction, directionToCamera);

    cvar arrowDirection = direction * arrowSize;
    cvar arrowOffset = arrowSize * 0.5f;

    DrawLine(start, end);
    DrawLine(end, end - arrowDirection + arrowOffsetAxis * arrowOffset);
    DrawLine(end, end - arrowDirection - arrowOffsetAxis * arrowOffset);
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
    var batch = m_instance->GetBatch(); // NOTE: The batch vertex/index count limit will be probably overshoot right there, but whatever.
    rvar vertexList = batch->GetVertexList();
    rvar indexList = batch->GetIndexList();

    cvar index = vertexList.Count();

    for (cvar vertexOffset : DebugMesh::m_cubeVertices)
    {
        var point = center + vertexOffset * halfSize;

        m_instance->TransformPoint(point);

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

void DebugDraw::DrawWireFrustum(const BoundingFrustum& frustum)
{
    cvar frustumCorners = frustum.GetCorners();

    DrawLine(frustumCorners[0], frustumCorners[1]);
    DrawLine(frustumCorners[1], frustumCorners[2]);
    DrawLine(frustumCorners[2], frustumCorners[3]);
    DrawLine(frustumCorners[3], frustumCorners[0]);

    DrawLine(frustumCorners[4], frustumCorners[5]);
    DrawLine(frustumCorners[5], frustumCorners[6]);
    DrawLine(frustumCorners[6], frustumCorners[7]);
    DrawLine(frustumCorners[7], frustumCorners[4]);

    DrawLine(frustumCorners[0], frustumCorners[4]);
    DrawLine(frustumCorners[1], frustumCorners[5]);
    DrawLine(frustumCorners[2], frustumCorners[6]);
    DrawLine(frustumCorners[3], frustumCorners[7]);
}

void DebugDraw::DrawSphere(const Vector3& center, float radius)
{
    // TODO: Draw sphere
}

void DebugDraw::DrawWireSphere(const Vector3& center, float radius)
{
    // TODO: Draw wire sphere - two circles on planes: XZ and [ZY or XY - based on camera angles]
}
