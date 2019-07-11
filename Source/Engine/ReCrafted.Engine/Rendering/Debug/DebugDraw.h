// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Common/List.h"
#include "Rendering/Shader.h"
#include "Rendering/RenderingComponent.h"

API_USING("ReCrafted.API.Mathematics")

/// <summary>
///     The DebugDraw class. Provides drawing functions for debugging.
/// </summary>
API_CLASS(public, static, noinherit)
class DebugDraw : public RenderingComponent<DebugDraw>
{
    friend class RenderingManager;
    API_CLASS_BODY()

private:
    const uint m_maxBatches = 128;

private:
    ALIGN(4) struct Vertex
    {
        Vector3 position = {};
        Vector4 color = {};

        Vertex(const Vector3& _position, const Vector4& _color) : position(_position), color(_color) { }
    };
    static_assert(sizeof(Vertex) == 28, "Vertex structure size must be 28 byte long");

    ALIGN(4) struct Point
    {
        Vector3 position = {};
        Vector4 color = {};

        Point(const Vector3& _position, const Vector4& _color) : position(_position), color(_color) { }
    };
    static_assert(sizeof(Point) == 28, "Point structure size must be 28 byte long");

    class Batch
    {
    public:
        static const uint maxPointsPerBatch = 65536;

        static const uint maxVerticesPerBatch = maxPointsPerBatch;
        static const uint maxIndicesPerBatch = (maxVerticesPerBatch / 4) * 6;

    private:
        List<Point> m_lines = {};
        List<Vertex> m_vertices = {};
        List<uint> m_indices = {};

    public:
        /// <summary>
        ///     Default Batch constructor.
        /// </summary>
        Batch()
        {
            // Reserve memory for vertices and lines.
            m_lines.Reserve(maxPointsPerBatch);
            m_vertices.Reserve(maxVerticesPerBatch);
            m_indices.Reserve(maxIndicesPerBatch);
        }

        /// <summary>
        ///     Default Batch destructor.
        /// </summary>
        ~Batch()
        {
            m_lines.Release();
            m_vertices.Release();
            m_indices.Release();
        }

    public:

        /// <summary>
        ///     Clears the batch.
        /// </summary>
        void Clear()
        {
            // Clear vertices and lines
            m_lines.Clear();
            m_vertices.Clear();
            m_indices.Clear();
        }

        /// <summary>
        ///     Returns true when this batch has some proper data to draw a shape.
        /// </summary>
        bool HasData() const
        {
            return m_lines.Count() >= 2 || m_vertices.Count() >= 3;
        }

        /// <summary>
        ///     Returns false when there is no more space to push any new data.
        /// </summary>
        bool IsFull() const
        {
            return m_lines.Count() >= maxPointsPerBatch - 2
                || m_vertices.Count() >= maxVerticesPerBatch - 3;
        }

        /// <summary>
        ///     Gets line list from this batch.
        /// </summary>
        List<Point>& GetLineList()
        {
            return m_lines;
        }

        /// <summary>
        ///     Gets vertex list from this batch.
        /// </summary>
        List<Vertex>& GetVertexList()
        {
            return m_vertices;
        }

        /// <summary>
        ///     Gets index list from this batch.
        /// </summary>
        List<uint>& GetIndexList()
        {
            return m_indices;
        }

    public:
        inline bool operator==(const Batch& rhs)
        {
            return this == &rhs;
        }
    };

private:
    List<Batch> m_batches {};
    Batch* m_currentBatch = nullptr;
    Vector4 m_currentColor{};
    Matrix m_matrix = Matrix::Identity;

    Shader* m_debugShader = nullptr;
    Renderer::VertexBufferHandle m_linesVB = {};
    Renderer::VertexBufferHandle m_trianglesVB = {};
    Renderer::IndexBufferHandle m_trianglesIB = {};

protected:
    void Initialize() override;
    void Shutdown() override;

    void Render() override;

    void RenderLines(Batch& batch) const;
    void RenderTriangles(Batch& batch) const;

    void TransformPoint(Vector3& point);

    FORCEINLINE void InternalDrawLine(const Vector3& start, const Vector3& end);

    Batch* GetBatch();

public:
    RenderingComponentStage GetStage() override
    {
        return RenderingComponentStage::Final;
    }

public:
    /// <summary>
    ///     Sets current debug draw render color.
    /// </summary>
    API_PROPERTY(noprefix)
    static void SetColor(const Color& color);

    /// <summary>
    ///     Gets current debug draw render color.
    /// </summary>
    API_PROPERTY(noprefix)
    static Color GetColor();

    /// <summary>
    ///     Sets matrix that will be used for vertex transformation.
    /// </summary>
    API_PROPERTY(noprefix)
    static void SetMatrix(const Matrix& matrix);

    /// <summary>
    ///     Gets current debug draw render transformation matrix.
    /// </summary>
    API_PROPERTY(noprefix)
    static Matrix& GetMatrix();

    /// <summary>
    ///     Draws arrow at the end point with line.
    /// </summary>
    /// <param name="start">The line start.</param>
    /// <param name="end">The line end and arrow position.</param>
    /// <param name="arrowSize">The arrow length.</param>
    API_FUNCTION()
    static void DrawArrow(const Vector3& start, const Vector3& end, float arrowSize = 0.25f);

    /// <summary>
    ///     Draws line.
    /// </summary>
    /// <param name="start">The line start.</param>
    /// <param name="end">The line end.</param>
    API_FUNCTION()
    static void DrawLine(const Vector3& start, const Vector3& end);

    /// <summary>
    ///     Draws box using given bounds.
    /// </summary>
    /// <param name="bounds">The box bounds.</param>
    API_FUNCTION()
    static void DrawBox(const BoundingBox& bounds)
    {
        DrawBox(bounds.center, bounds.size);
    }

    /// <summary>
    ///     Draws box using given position (center) and size.
    /// </summary>
    /// <param name="center">The center position.</param>
    /// <param name="size">The size of the box.</param>
    API_FUNCTION()
    static void DrawBox(const Vector3& center, const Vector3& size);
    
    /// <summary>
    ///     Draws wire box using given position (center) and size.
    /// </summary>
    /// <param name="center">The center position.</param>
    /// <param name="size">The size of the box.</param>
    API_FUNCTION()
    static void DrawWireBox(const Vector3& center, const Vector3& size);

    /// <summary>
    ///     Draws wire box using given bounds.
    /// </summary>
    /// <param name="bounds">The box bounds.</param>
    API_FUNCTION()
    static void DrawWireBox(const BoundingBox& bounds)
    {
        DrawWireBox(bounds.center, bounds.size);
    }

    /// <summary>
    ///     Draws given frustum as lines between every corner.
    /// </summary>
    /// <param name="frustum">The frustum.</param>
    API_FUNCTION()
    static void DrawWireFrustum(const BoundingFrustum& frustum);

    /// <summary>
    ///     Draws sphere at given position.
    /// </summary>
    /// <param name="center">The sphere draw position.</param>
    /// <param name="radius">The sphere radius.</param>
    API_FUNCTION()
    static void DrawSphere(const Vector3& center, float radius);

    /// <summary>
    ///     Draws wire sphere at given position.
    /// </summary>
    /// <param name="center">The sphere draw position.</param>
    /// <param name="radius">The sphere radius.</param>
    API_FUNCTION()
    static void DrawWireSphere(const Vector3& center, float radius);

    /// <summary>
    ///     Draws wire circle.
    /// </summary>
    /// <param name="center">The circle center point.</param>
    /// <param name="majorAxis">The major axis point.</param>
    /// <param name="minorAxis">The minor axis point.</param>
    API_FUNCTION()
    static void DrawWireCircle(const Vector3& center, const Vector3& majorAxis, const Vector3& minorAxis);
};