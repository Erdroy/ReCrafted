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
        if (ImGui::MenuItem(m_profilingEnabled ? "Pause profiler" : "Resume profiler"))
        {
            ToggleProfiling();
        }
        ImGui::EndMenuBar();
    }
    
    for(var thread : m_threads)
        DrawThreadProfiles(thread);

    ImGui::End();
}

void Profiler::DrawThreadProfiles(ThreadData* thread)
{
    rvar lock = thread->dataLock;
    ScopeLock(lock);

    // Reset selected frame if not paused
    if (m_profilingEnabled)
        thread->selectedFrame = -1;

    ImGui::PushID(static_cast<int>(std::hash<std::thread::id>{}(thread->threadId)));
    if(ImGui::CollapsingHeader(thread->threadName))
    {
        float frameTimes[NumProfiledFrames] = {};
        auto avgFrameTime = 0.0f;
        auto maxFrameTime = 0.0f;
        auto minFrameTime = FLT_MAX;
        for (var i = 0u; i < thread->frames.size(); i++)
        {
            cvar frameTime = thread->frames[i].time_ms;

            maxFrameTime = std::max(maxFrameTime, frameTime);
            minFrameTime = std::min(minFrameTime, frameTime);
            avgFrameTime += frameTime;
            frameTimes[i] = frameTime;
        }
        
        avgFrameTime /= static_cast<float>(thread->frames.size());

        ImGui::PushItemWidth(-1);
        cvar hoveredFrame = ImGui::PlotLines("", frameTimes, NumProfiledFrames, 0, nullptr, FLT_MAX, FLT_MAX, ImVec2(0, 128));

        if (ImGui::IsMouseClicked(0) && hoveredFrame >= 0)
        {
            thread->selectedFrame = hoveredFrame;
            m_stopProfiling = true;
        }

        cvar graphSize = ImGui::GetItemRectSize();
        cvar startPos = ImGui::GetItemRectMin();

        // Sample line drawing
        cvar drawList = ImGui::GetWindowDrawList();

        // Draw max line
        drawList->AddLine(ImVec2(startPos.x, startPos.y + 10.0f), ImVec2(startPos.x + graphSize.x, startPos.y + 10.0f), 0xFF0000FF);
        drawList->AddText(ImVec2(startPos.x, startPos.y + 10.0f), 0xFFFFFFFF, (std::to_string(maxFrameTime) + std::string("ms (max)")).c_str());

        // Draw avg line
        drawList->AddLine(ImVec2(startPos.x, startPos.y + 60.0f), ImVec2(startPos.x + graphSize.x, startPos.y + 60.0f), 0xFF0000FF);
        drawList->AddText(ImVec2(startPos.x, startPos.y + 60.0f), 0xFFFFFFFF, (std::to_string(avgFrameTime) + std::string("ms (avg)")).c_str());

        // Draw min line
        drawList->AddLine(ImVec2(startPos.x, startPos.y + 100.0f), ImVec2(startPos.x + graphSize.x, startPos.y + 100.0f), 0xFF0000FF);
        drawList->AddText(ImVec2(startPos.x, startPos.y + 100.0f), 0xFFFFFFFF, (std::to_string(minFrameTime) + std::string("ms (min)")).c_str());

        if(thread->selectedFrame >= 0)
        {
            if (ImGui::CollapsingHeader(("Events##" + std::string(thread->threadName)).c_str()))
            {
                // TODO: Draw events of selected frame
            }
        }
    }
}
