// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef DEBUGDRAW_H
#define DEBUGDRAW_H

// includes
#include "ReCrafted.h"
#include "Core/EngineComponent.h"
#include "Core/Containers/Array.h"
#include "Core/Math/Math.h"
#include "Core/Math/Color.h"

/**
 * \brief The DebugDraw class. Provides drawing functions for debugging.
 */
class DebugDraw : public EngineComponent<DebugDraw>
{
    friend class Graphics;

private:
    const uint m_maxBatches = 128;

private:
    struct ALIGN(4) Vertex
    {
        Vector3 position = {};
        Color color = {};

        Vertex(const Vector3& _position, const Color& _color) : position(_position), color(_color) { }
    };

    struct ALIGN(4) Point
    {
        Vector3 position = {};
        Color color = {};

        Point(const Vector3& _position, const Color& _color) : position(_position), color(_color) { }
    };

    class Batch
    {
    private:
        const uint m_maxVerticesPerBatch = 65536;
        const uint m_maxPointsPerBatch = m_maxVerticesPerBatch;

    private:
        Array<Point> m_lines = {};
        Array<Vertex> m_vertices = {};

    public:
        /**
         * \brief Default Batch constructor.
         */
        Batch()
        {
            // Reserve memory for vertices and lines.
            m_vertices.Reserve(m_maxVerticesPerBatch);
            m_lines.Reserve(m_maxPointsPerBatch);
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
            return m_lines.Count() >= m_maxPointsPerBatch - 2
                || m_vertices.Count() >= m_maxVerticesPerBatch - 3;
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

    public:
        inline bool operator==(const Batch& rhs)
        {
            return this == &rhs;
        }
    };

private:
    Array<Batch> m_batches;
    RefPtr<Batch> m_currentBatch;
    Color m_currentColor;

public:
    DebugDraw()
    {
        // Add one batch
        m_batches = {};
        m_batches.Add({});
        m_currentBatch.reset(&m_batches[0]);
        m_currentColor = Color(0xFF0000FF);
    }

private:
    void OnInit() override;
    void OnLoad() override;
    void Update() override;

    void RenderLines(const Batch& batch);
    void RenderTriangles(const Batch& batch);

    void Render();
    void OnDispose() override;

    void DrawLine(const Vector3& start, const Vector3& end);

    RefPtr<Batch>& GetBatch();

public:
    static void SetColor(const Color& color);
    static void DrawBox(const Vector3& position, const Vector3& size);
};

#endif // DEBUGDRAW_H
