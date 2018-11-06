// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef DEBUGDRAW_H
#define DEBUGDRAW_H

// includes
#include "ReCrafted.h"
#include "Core/EngineComponent.h"
#include "Core/Containers/Array.h"
#include "Core/Math/Math.h"
#include "Core/Math/BoundingFrustum.h"
#include "Core/Math/Color.h"
#include "Renderer/Renderer.hpp"

/**
 * \brief The DebugDraw class. Provides drawing functions for debugging.
 */
class DebugDraw : public EngineComponent<DebugDraw>
{
    friend class Graphics;

private:
    SCRIPTING_API_IMPL()

private:
    const uint m_maxBatches = 128;

private:
    ALIGN(4) struct Vertex
    {
        Vector3 position = {};
        Vector4 color = {};

        Vertex(const Vector3& _position, const Vector4& _color) : position(_position), color(_color) { }
    };
    STATIC_ASSERT(sizeof(Vertex) == 28);

    ALIGN(4) struct Point
    {
        Vector3 position = {};
        Vector4 color = {};

        Point(const Vector3& _position, const Vector4& _color) : position(_position), color(_color) { }
    };
    STATIC_ASSERT(sizeof(Point) == 28);

    class Batch
    {
    public:
        static const uint maxPointsPerBatch = 65536;

        static const uint maxVerticesPerBatch = maxPointsPerBatch;
        static const uint maxIndicesPerBatch = (maxVerticesPerBatch / 4) * 6;

    private:
        Array<Point> m_lines = {};
        Array<Vertex> m_vertices = {};
        Array<uint> m_indices = {};

    public:
        /**
         * \brief Default Batch constructor.
         */
        Batch()
        {
            // Reserve memory for vertices and lines.
            m_lines.Reserve(maxPointsPerBatch);
            m_vertices.Reserve(maxVerticesPerBatch);
            m_indices.Reserve(maxIndicesPerBatch);
        }

        /**
         * \brief Default Batch destructor.
         */
        ~Batch()
        {
            m_lines.Release();
            m_vertices.Release();
            m_indices.Release();
        }

    public:
        /**
         * \brief Clears the batch.
         */
        void Clear()
        {
            // Clear vertices and lines
            m_lines.Clear();
            m_vertices.Clear();
            m_indices.Clear();
        }

        /**
         * \brief Returns true when this batch has some proper data to draw a shape.
         */
        bool HasData() const
        {
            return m_lines.Count() >= 2 || m_vertices.Count() >= 3;
        }

        /**
         * \brief Returns false when there is no more space to push any new data.
         */
        bool IsFull() const
        {
            return m_lines.Count() >= maxPointsPerBatch - 2
                || m_vertices.Count() >= maxVerticesPerBatch - 3;
        }

        /**
         * \brief Gets line list from this batch.
         */
        Array<Point>& GetLineList()
        {
            return m_lines;
        }

        /**
        * \brief Gets vertex list from this batch.
        */
        Array<Vertex>& GetVertexList()
        {
            return m_vertices;
        }

        /**
        * \brief Gets index list from this batch.
        */
        Array<uint>& GetIndexList()
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
    Array<Batch> m_batches;
    Batch* m_currentBatch;
    Vector4 m_currentColor;
    Matrix m_matrix;

    Renderer::ShaderHandle m_debugShader = {};
    Renderer::VertexBufferHandle m_linesVB = {};
    Renderer::VertexBufferHandle m_trianglesVB = {};
    Renderer::IndexBufferHandle m_trianglesIB = {};

public:
    DebugDraw()
    {
        // Add one batch
        m_batches = {};
        m_batches.Add({});
        m_currentBatch = &m_batches[0];
        m_currentColor = Color(0xFF0000FF).ToVector4();
    }

private:
    void OnInit() override;

    void RenderLines(Batch& batch);
    void RenderTriangles(Batch& batch);

    void Render();
    void OnDispose() override;

    void TransformPoint(Vector3& point);

    FORCEINLINE void InternalDrawLine(const Vector3& start, const Vector3& end);

    Batch* GetBatch();

public:
    /**
     * \brief Sets current debug draw render color.
     * \param color The color.
     */
    static void SetColor(const Color& color);

    /**
    * \brief Gets current debug draw render color.
    */
    static Color GetColor();

    /**
     * \brief Sets matrix that will be used for vertex transformation.
     * \param matrix The matrix.
     */
    static void SetMatrix(const Matrix& matrix);

    /**
    * \brief Gets current debug draw render transformation matrix.
    */
    static Matrix& GetMatrix();

    /**
     * \brief Draws arrow at the end point with line.
     * \param start The line start.
     * \param end The line end and arrow position.
     * \param arrowSize The arrow length.
     */
    static void DrawArrow(const Vector3& start, const Vector3& end, float arrowSize = 0.25f);

    /**
    * \brief Draws line.
    * \param start The line start.
    * \param end The line end.
    */
    static void DrawLine(const Vector3& start, const Vector3& end);

    /**
    * \brief Draws box using given bounds.
    * \param bounds The box bounds.
    */
    static void DrawBox(const BoundingBox& bounds)
    {
        DrawBox(bounds.center, bounds.size);
    }

    /**
     * \brief Draws box using given position (center) and size.
     * \param center The center position.
     * \param size The size of the box.
     */
    static void DrawBox(const Vector3& center, const Vector3& size);

    /**
    * \brief Draws wire box using given position (center) and size.
    * \param center The center position.
    * \param size The size of the box.
    */
    static void DrawWireBox(const Vector3& center, const Vector3& size);

    /**
    * \brief Draws wire box using given bounds.
    * \param bounds The box bounds.
    */
    static void DrawWireBox(const BoundingBox& bounds)
    {
        DrawWireBox(bounds.center, bounds.size);
    }

    /**
     * \brief Draws given frustum as lines between every corner.
     * \param frustum The frustum.
     */
    static void DrawWireFrustum(const BoundingFrustum& frustum);

    /**
     * \brief Draws sphere at given position.
     * \param center The sphere draw position.
     * \param radius The sphere radius.
     */
    static void DrawSphere(const Vector3& center, float radius);

    /**
    * \brief Draws wire sphere at given position.
    * \param center The sphere draw position.
    * \param radius The sphere radius.
    */
    static void DrawWireSphere(const Vector3& center, float radius);
};

#endif // DEBUGDRAW_H
