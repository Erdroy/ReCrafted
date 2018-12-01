// ReCrafted (c) 2016-2018 Always Too Late

#include "Profiler.h"
#include "imgui.h"

std::vector<float> m_data(256);

Profiler::ThreadData* Profiler::GetCurrentThreadData()
{
    cvar it = m_threadMap.find(std::this_thread::get_id());
    if(it != m_threadMap.end())
        return it->second;
    return nullptr;
}

void Profiler::DrawWindow()
{
    ImGui::Begin("Profiler", nullptr, ImGuiWindowFlags_MenuBar);

    if(ImGui::BeginMenuBar())
    {
        if (ImGui::MenuItem(m_profilingEnabled ? "Resume profile" : "Pause profile"))
        {
            ToggleProfiling();
        }
        ImGui::EndMenuBar();
    }
    
    for(var thread : m_threads)
        DrawThread(thread);

    ImGui::End();
}

void Profiler::DrawThread(ThreadData* thread)
{
    rvar lock = thread->dataLock;
    ScopeLock(lock);

    ImGui::PushID(static_cast<int>(std::hash<std::thread::id>{}(thread->threadId)));
    if(ImGui::CollapsingHeader(thread->threadName, ImGuiTreeNodeFlags_Framed))
    {
        // Temporary
        float frameTimes[120 * ProfileSeconds] = {};
        for (var i = 0u; i < thread->frames.size(); i++)
        {
            frameTimes[i] = thread->frames[i].time_ms;
        }

        ImGui::PushItemWidth(-1);
        ImGui::PlotLines("", frameTimes, 120 * ProfileSeconds, 0, nullptr, FLT_MAX, FLT_MAX, ImVec2(0, 128));
        cvar graphSize = ImGui::GetItemRectSize();
        cvar startPos = ImGui::GetItemRectMin();

        // TODO: Avg/Max time line

        // Sample line drawing
        cvar drawList = ImGui::GetWindowDrawList();
        drawList->AddLine(ImVec2(startPos.x, startPos.y + 10.0f), ImVec2(startPos.x + graphSize.x, startPos.y + 10.0f), 0xFF0000FF);
    }
}
