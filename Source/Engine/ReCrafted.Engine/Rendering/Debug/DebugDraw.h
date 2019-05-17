// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Rendering/Shader.h"
#include "Common/List.h"
#include "Rendering/RenderingComponent.h"

/**
 * \brief The DebugDraw class. Provides drawing functions for debugging.
 */
class DebugDraw : public RenderingComponent<DebugDraw>
{
    friend class Graphics;

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
        List<Point>& GetLineList()
        {
            return m_lines;
        }

        /**
        * \brief Gets vertex list from this batch.
        */
        List<Vertex>& GetVertexList()
        {
            return m_vertices;
        }

        /**
        * \brief Gets index list from this batch.
        */
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
    List<Batch> m_batches;
    Batch* m_currentBatch = nullptr;
    Vector4 m_currentColor{};
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
        m_currentColor = Color(0xFF0000FF).ToVector();
    }

protected:
    void Initialize() override;
    void Shutdown() override;

    void RenderLines(Batch& batch);
    void RenderTriangles(Batch& batch);

    void Render();

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

    /**
     * \brief Draws wire circle.
     * \param center The circle center point.
     * \param majorAxis The major axis point.
     * \param minorAxis The minor axis point.
     */
    static void DrawWireCircle(const Vector3& center, const Vector3& majorAxis, const Vector3& minorAxis);
};