// ReCrafted (c) 2016-2018 Always Too Late

#include "Profiler.h"
#include "imgui.h"

void Profiler::DrawWindow()
{
    ImGui::Begin("Profiler");

    for(var thread : m_threads)
        DrawThread(thread);

    ImGui::End();
}

void Profiler::DrawThread(ProfilerThread* thread)
{

}
