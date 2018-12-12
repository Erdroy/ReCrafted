// ReCrafted (c) 2016-2018 Always Too Late

#include "Profiler.h"
#include "imgui.h"

void Profiler::DrawWindow()
{
    ImGui::Begin("Profiler", nullptr, ImGuiWindowFlags_MenuBar);

    if(ImGui::BeginMenuBar())
    {
        if (ImGui::MenuItem(m_profilingEnabled ? "[P] Pause profiler" : "[P] Resume profiler"))
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

        if (ImGui::IsMouseClicked(0) && hoveredFrame >= 0 && hoveredFrame < static_cast<int>(thread->frames.size()))
        {
            thread->selectedFrame = hoveredFrame;
            m_stopProfiling = true;
        }

        cvar graphSize = ImGui::GetItemRectSize();
        cvar startPos = ImGui::GetItemRectMin();

        // Sample line drawing
        cvar drawList = ImGui::GetWindowDrawList();

        // Draw max line
        drawList->AddLine(ImVec2(startPos.x, startPos.y + 10.0f), ImVec2(startPos.x + graphSize.x, startPos.y + 10.0f), 0xFF1000FF);
        drawList->AddText(ImVec2(startPos.x, startPos.y + 10.0f), 0xFFFFFFFF, (std::to_string(maxFrameTime) + std::string("ms (max)")).c_str());

        // Draw avg line
        drawList->AddLine(ImVec2(startPos.x, startPos.y + 60.0f), ImVec2(startPos.x + graphSize.x, startPos.y + 60.0f), 0xFF1000FF);
        drawList->AddText(ImVec2(startPos.x, startPos.y + 60.0f), 0xFFFFFFFF, (std::to_string(avgFrameTime) + std::string("ms (avg)")).c_str());

        // Draw min line
        drawList->AddLine(ImVec2(startPos.x, startPos.y + 100.0f), ImVec2(startPos.x + graphSize.x, startPos.y + 100.0f), 0xFF1000FF);
        drawList->AddText(ImVec2(startPos.x, startPos.y + 100.0f), 0xFFFFFFFF, (std::to_string(minFrameTime) + std::string("ms (min)")).c_str());

        if(thread->selectedFrame >= 0)
        {
            if (ImGui::CollapsingHeader(("Events##" + std::string(thread->threadName)).c_str()))
            {
                // Select frame
                crvar selectedFrame = thread->frames[thread->selectedFrame];

                // Compile profiles
                Array<ProfileTreeEntry> treeProfiles;
                CompileProfiles(selectedFrame.profiles, treeProfiles);

                // Setup columns
                ImGui::Columns(4);
                ImGui::SetColumnWidth(1, 100.0f);
                ImGui::SetColumnWidth(2, 100.0f);
                ImGui::SetColumnWidth(3, 100.0f);

                // Draw Header
                ImGui::Text("Overview");
                ImGui::NextColumn();

                ImGui::Text("Total");
                ImGui::NextColumn();

                ImGui::Text("Time ms");
                ImGui::NextColumn();

                ImGui::Text("Calls");
                ImGui::NextColumn();

                ImGui::Separator();

                // Draw compiled profiles in a tree
                for (crvar event : treeProfiles)
                {
                    DrawThreadProfile(event);
                }

                ImGui::Columns(1);
            }
        }
    }

    ImGui::PopID();
}

void Profiler::DrawThreadProfile(const ProfileTreeEntry& event)
{
    cvar open = ImGui::TreeNodeEx(event.name.c_str(), event.children.Empty() ? ImGuiTreeNodeFlags_Leaf : 0);

    ImGui::NextColumn();

    ImGui::Text("%.2f %s", 0.0f /*event.time / selectedFrame.time_ms * 100.0f*/, "%");
    ImGui::NextColumn();

    ImGui::Text("%.3f %s", event.time, "ms");
    ImGui::NextColumn();

    ImGui::Text("%d", event.callNum);
    ImGui::NextColumn();

    if (open)
    {
        // Draw all children if this node is open
        for (var childEvent : event.children)
        {
            DrawThreadProfile(childEvent);
        }

        // Pop tree
        ImGui::TreePop();
    }
}
