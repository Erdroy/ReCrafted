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
            if (m_profilingEnabled)
                m_profilingEnabled = false;
            else
                m_profilingEnabled = true;
        }
        ImGui::EndMenuBar();
    }
    
    for(var thread : m_threads)
        DrawThread(thread);

    ImGui::End();
}

void Profiler::DrawThread(ThreadData* thread)
{
    // Temporary
    float frameTimes[120 * ProfileSeconds] = {};
    for(var i = 0u; i < thread->frames.size(); i ++)
    {
        frameTimes[i] = thread->frames[i].time_ms;
    }

    ImGui::PushItemWidth(-1);
    ImGui::PlotHistogram("", frameTimes, 120 * ProfileSeconds);
}
