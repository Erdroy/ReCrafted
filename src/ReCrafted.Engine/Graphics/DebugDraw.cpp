// ReCrafted (c) 2016-2018 Always Too Late

#include "DebugDraw.h"

SINGLETON_IMPL(DebugDraw)

void DebugDraw::OnInit()
{
    m_lines.Reserve(4096); // 2048 lines at the start
}

void DebugDraw::OnLoad()
{
}

void DebugDraw::Update()
{
}

void DebugDraw::Render()
{
    

    // Clear lines at the end
    m_lines.Clear();
}

void DebugDraw::OnDispose()
{
}

void DebugDraw::DrawBox(const Vector3& position, const Vector3& size)
{
    m_instance->m_lines.Add(position);
    m_instance->m_lines.Add(position + Vector3::Up());
}
