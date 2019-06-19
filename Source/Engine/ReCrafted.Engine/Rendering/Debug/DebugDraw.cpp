// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "DebugDraw.h"
#include "Content/ContentManager.h"
#include "Rendering/Camera.h"
#include "Rendering/RenderingManager.h"
#include "Rendering/DrawMode.h"
#include "Rendering/RenderingBase.h"

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

void DebugDraw::Initialize()
{
    // Add one batch
    m_batches.Add({});
    m_currentBatch = &m_batches[0];
    m_currentColor = Color::FromHex(0xFF0000FF).ToVector();

    m_debugShader = ContentManager::LoadAsset<Shader>("Shaders/Debug");

    m_linesVB = Renderer::CreateVertexBuffer(Batch::maxPointsPerBatch, sizeof(Point), true);
    m_trianglesVB = Renderer::CreateVertexBuffer(Batch::maxVerticesPerBatch, sizeof(Vertex), true);
    m_trianglesIB = Renderer::CreateIndexBuffer(Batch::maxIndicesPerBatch, nullptr, true, true);
}

void DebugDraw::Shutdown()
{
    // Note: We cannot unload the loaded asset (m_debugShader) as we are way after the ContentManager was unloaded
    // (with all assets) and thus, we don't have to do it.

    m_batches.Release();

    Renderer::DestroyVertexBuffer(m_linesVB);
    Renderer::DestroyVertexBuffer(m_trianglesVB);
    Renderer::DestroyIndexBuffer(m_trianglesIB);
}

void DebugDraw::Render()
{
    //Profiler::BeginProfile(__FUNCTION__);

    // Set shader values
    auto mvpMatrix = Camera::GetMainCamera()->GetViewProjection();
    Renderer::SetShaderValue(m_debugShader->GetHandle(), 0, 0, &mvpMatrix, sizeof(Matrix));

    // Apply shader
    Renderer::ApplyShader(m_debugShader->GetHandle(), 0);

    for (auto& batch : m_batches)
    {
        RenderTriangles(batch);
        RenderLines(batch);

        batch.Clear();
    }

    // We don't want to remove batches, but only reset to the first one
    m_currentBatch = &m_batches[0];

    // Reset matrix
    SetMatrix(Matrix::Identity);

    //Profiler::EndProfile();
}

void DebugDraw::RenderLines(Batch& batch) const
{
    auto& lineList = batch.GetLineList();
    const auto pointCount = lineList.Count();

    if (pointCount == 0)
        return;

    // Set line draw mode
    RenderingManager::SetDrawMode(DrawMode::DebugDrawLines);

    // Allocate vertex buffer
    const auto dataSize = pointCount * sizeof(Point);
    const auto data = Renderer::Allocate(dataSize);

    // Copy line list into data
    memcpy_s(data, dataSize, lineList.Data(), dataSize);

    // Update vertex buffer
    Renderer::UpdateVertexBuffer(m_linesVB, data, static_cast<uint>(dataSize), 0u);

    // Apply vertex buffer
    Renderer::ApplyVertexBuffer(m_linesVB);

    // Draw
    Renderer::Draw(pointCount);
}

void DebugDraw::RenderTriangles(Batch& batch) const
{
    auto& vertexList = batch.GetVertexList();
    auto& indexList = batch.GetIndexList();
    const auto vertexCount = vertexList.Count();
    const auto indexCount = indexList.Count();

    if (vertexCount == 0)
        return;

    // Set triangle draw mode
    RenderingManager::SetDrawMode(DrawMode::DebugDrawTriangles);

    // Allocate vertex buffer
    const auto vertexDataSize = vertexCount * sizeof(Vertex);
    const auto vertexData = Renderer::Allocate(vertexDataSize);

    // Copy vertex list into data
    memcpy_s(vertexData, vertexDataSize, vertexList.Data(), vertexDataSize);

    // Allocate index buffer
    const auto indexDataSize = indexCount * sizeof(uint);
    const auto indexData = Renderer::Allocate(indexDataSize);

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

void DebugDraw::TransformPoint(Vector3 & point)
{
    if (m_matrix.IsIdentity())
        return;

    point = Vector3::Transform(point, m_matrix);
}

void DebugDraw::InternalDrawLine(const Vector3 & start, const Vector3 & end)
{
    // Add new line to the current batch
    auto batch = GetBatch();
    auto& lineList = batch->GetLineList();

    auto point1 = start;
    auto point2 = end;

    GetInstance()->TransformPoint(point1);
    GetInstance()->TransformPoint(point2);

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
        const auto idx = m_batches.IndexOf(*m_currentBatch);

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
            m_currentBatch = &m_batches[m_batches.Count() - 1];
        }
    }

    return m_currentBatch;
}

void DebugDraw::SetColor(const Color& color)
{
    ASSERT(IS_MAIN_THREAD());

    GetInstance()->m_currentColor = color.ToVector();
}

Color DebugDraw::GetColor()
{
    return Color::FromVector(GetInstance()->m_currentColor);
}

void DebugDraw::SetMatrix(const Matrix & matrix)
{
    GetInstance()->m_matrix = matrix;
}

Matrix& DebugDraw::GetMatrix()
{
    return GetInstance()->m_matrix;
}

void DebugDraw::DrawArrow(const Vector3 & start, const Vector3 & end, float arrowSize)
{
    const auto direction = Vector3::Normalize(end - start);
    const auto directionToCamera = Vector3::Normalize(end - Camera::GetMainCamera()->Position());
    const auto arrowOffsetAxis = Vector3::Cross(direction, directionToCamera);

    const auto arrowDirection = direction * arrowSize;
    const auto arrowOffset = arrowSize * 0.5f;

    DrawLine(start, end);
    DrawLine(end, end - arrowDirection + arrowOffsetAxis * arrowOffset);
    DrawLine(end, end - arrowDirection - arrowOffsetAxis * arrowOffset);
}

void DebugDraw::DrawLine(const Vector3 & start, const Vector3 & end)
{
    ASSERT(IS_MAIN_THREAD());

    GetInstance()->InternalDrawLine(start, end);
}

void DebugDraw::DrawBox(const Vector3 & center, const Vector3 & size)
{
    ASSERT(IS_MAIN_THREAD());

    const auto halfSize = size * 0.5f;

    // Add new triangles to the current batch
    auto batch = GetInstance()->GetBatch(); // NOTE: The batch vertex/index count limit will be probably overshoot right there, but whatever.
    auto& vertexList = batch->GetVertexList();
    auto& indexList = batch->GetIndexList();

    const auto index = vertexList.Count();

    for (const auto vertexOffset : DebugMesh::m_cubeVertices)
    {
        auto point = center + vertexOffset * halfSize;

        GetInstance()->TransformPoint(point);

        vertexList.Add({ point, GetInstance()->m_currentColor });
    }

    for (const auto indexOffset : DebugMesh::m_cubeIndices)
    {
        indexList.Add(uint(index + indexOffset));
    }
}

void DebugDraw::DrawWireBox(const Vector3 & center, const Vector3 & size)
{
    const auto halfSize = size * 0.5f;

    for (auto&& edge : DebugMesh::m_wireCubeEdges)
        GetInstance()->DrawLine(center + edge[0] * halfSize, center + edge[1] * halfSize);
}

void DebugDraw::DrawWireFrustum(const BoundingFrustum & frustum)
{
    const auto frustumCorners = frustum.GetCorners();

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

void DebugDraw::DrawSphere(const Vector3 & center, float radius)
{
    // TODO: Draw sphere
}

void DebugDraw::DrawWireSphere(const Vector3 & center, const float radius)
{
    DrawWireCircle(center, Vector3::UnitX * radius, Vector3::UnitZ * radius);
    DrawWireCircle(center, Vector3::UnitX * radius, Vector3::UnitY * radius);
    DrawWireCircle(center, Vector3::UnitY * radius, Vector3::UnitZ * radius);
}

void DebugDraw::DrawWireCircle(const Vector3 & center, const Vector3 & majorAxis, const Vector3 & minorAxis)
{
    static const size_t ringSegments = 16;

    // Add new triangles to the current batch

    auto batch = GetInstance()->GetBatch();
    auto& lineList = batch->GetLineList();

    const auto angleDelta = Math::TwoPi / float(ringSegments);

    const auto cosDelta = Vector3(Math::Cos(angleDelta));
    const auto sinDelta = Vector3(Math::Sin(angleDelta));
    auto incrementalSin = Vector3::Zero;
    auto incrementalCos = Vector3::One;

    auto firstPoint = Vector3::Zero;
    auto lastPoint = Vector3::Zero;

    for (auto i = 0u; i < ringSegments; i++)
    {
        auto point = majorAxis * incrementalCos + center;
        point += minorAxis * incrementalSin;

        // Save the first point, to finish up the ring later
        if (i == 0u)
        {
            firstPoint = point;
        }
        else
        {
            // Push new line point
            GetInstance()->DrawLine(point, lastPoint);
        }

        lastPoint = point;

        const auto newCos = incrementalCos * cosDelta - incrementalSin * sinDelta;
        const auto newSin = incrementalCos * sinDelta + incrementalSin * cosDelta;

        incrementalCos = newCos;
        incrementalSin = newSin;
    }

    // Push new line point
    GetInstance()->DrawLine(firstPoint, lastPoint);
}
