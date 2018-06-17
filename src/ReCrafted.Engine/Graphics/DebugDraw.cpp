// ReCrafted (c) 2016-2018 Always Too Late

#include "DebugDraw.h"

SINGLETON_IMPL(DebugDraw)

void DebugDraw::OnInit()
{
}

void DebugDraw::OnLoad()
{
}

void DebugDraw::Update()
{
}

void DebugDraw::RenderLines(const Batch& batch)
{

}

void DebugDraw::RenderTriangles(const Batch& batch)
{

}

void DebugDraw::Render()
{
    for (rvar batch : m_batches)
    {
        RenderLines(batch);
        RenderTriangles(batch);

        batch.Clear();
    }

    // We don't want to remove batches, but only reset to the first one
    m_currentBatch.reset(&m_batches.First());
}

void DebugDraw::OnDispose()
{
}

void DebugDraw::DrawLine(const Vector3& start, const Vector3& end)
{
    // Add new line to the current batch
    rvar batch = GetBatch();
    rvar lineList = batch->GetLineList();

    lineList.Add(Point{start, m_currentColor});
    lineList.Add(Point{end, m_currentColor});
}

RefPtr<DebugDraw::Batch>& DebugDraw::GetBatch()
{
    if (m_currentBatch->IsFull())
    {
        ASSERT(m_batches.Count() < m_maxBatches);

        // Check if there is next batch available
        // - if not: create new batch
        // Use new batch
        cvar idx = m_batches.IndexOf(*m_currentBatch.get());

        if (idx < static_cast<int>(m_batches.Size()) - 1)
        {
            // Select next batch
            m_currentBatch.reset(&m_batches[idx + 1]);
        }
        else
        {
            // Create new batch
            m_batches.Add({});

            // Set the new batch as first
            m_currentBatch.reset(&m_batches.Last());
        }
    }

    return m_currentBatch;
}

void DebugDraw::SetColor(const Color& color)
{
    ASSERT(IS_MAIN_THREAD());

    m_instance->m_currentColor = color;
}

void DebugDraw::DrawBox(const Vector3& position, const Vector3& size)
{
    ASSERT(IS_MAIN_THREAD());

    m_instance->DrawLine(position, position + Vector3::Up());
}
