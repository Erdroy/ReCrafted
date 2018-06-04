// ReCrafted (c) 2016-2018 Always Too Late

#include "Profiler.h"
#include "Common/Time.h"
#include "Common/Input/Input.h"
#include "Core/Logger.h"
#include "Core/ByteFormat.h"
#include "Graphics/UI/UI.h"
#include "Graphics/Font.h"

#include <mono/metadata/mono-gc.h>
#include "Graphics/Camera.h"

SINGLETON_IMPL(Profiler)

Array<Profiler::Profile> Profiler::m_profiles;
Array<Profiler::Profile*> Profiler::m_profileStack;
int Profiler::m_profileCount;

Color GetFPSLineColor(int fps)
{
    var color = Color(0x00FF00FF);

    if (fps < 59)
        color = Color(0xFFFF00FF);

    if (fps < 29)
        color = Color(0xFF0000FF);

    return color;
}

bool Profiler::ProfileSort(const Profile& lhs, const Profile& rhs)
{
    return lhs.order > rhs.order;
}

void Profiler::OnInit()
{
    var debugFont = TEXT_CONST("../assets/fonts/VeraMono.ttf");

    m_drawDebugScreen = false;

    m_debugFont = new Font();
    m_debugFont->LoadFont(debugFont, 12);
}

void Profiler::OnDispose()
{
    // Destroy font
    Object::Destroy(m_debugFont);
}

void Profiler::Update()
{
    m_frames++;
    m_lineOffset = 10.0f;

    if (Time::CurrentTime() - m_lastFPSCountTime > 1.0f)
    {
        m_fps = m_frames;
        m_frames = 0;
        m_lastFPSCountTime = Time::CurrentTime();
    }

    if (Input::IsKeyDown(Key_F9))
        m_drawDebugScreen = !m_drawDebugScreen;

    if (Input::IsKeyDown(Key_P) && m_drawDebugScreen)
        m_drawProfiles = !m_drawProfiles;
}

void Profiler::DrawTextLine(Text text, Color color)
{
    UI::SetColor(color);
    UI::DrawText(m_debugFont, text, Vector2(m_horiOffset, m_lineOffset));
    m_lineOffset += static_cast<float>(m_debugFont->GetSize());
}

void Profiler::MakeLineSpace(int lines)
{
    m_lineOffset += static_cast<float>(m_debugFont->GetSize()) * lines;
}

void Profiler::DrawDebugScreen()
{
    if (!m_drawDebugScreen)
        return;

    BeginProfile("Profile");
    EndProfile();

    BeginProfile("Profiler Draw (self profile)", 0.5f, 1.0f);
    {
        Renderer::RenderStatistics renderStats = {};
        Renderer::GetRenderStatistics(&renderStats);

        rpmalloc_global_statistics_t statistics;
        rpmalloc_global_statistics(&statistics);

        float gcUsedSize;
        float gcHeapSize;
        cvar gcUsedSizeUnit = ByteFormat::Format(mono_gc_get_used_size(), &gcUsedSize);
        cvar gcHeapSizeUnit = ByteFormat::Format(mono_gc_get_heap_size(), &gcHeapSize);

        float memStatsCached;
        float memStatsMapped;
        float memStatsMappedTotal;
        float memStatsUnmappedTotal;
        cvar memStatsCachedUnit = ByteFormat::Format(statistics.cached, &memStatsCached);
        cvar memStatsMappedUnit = ByteFormat::Format(statistics.mapped, &memStatsMapped);
        cvar memStatsMappedTotalUnit = ByteFormat::Format(statistics.mapped_total, &memStatsMappedTotal);
        cvar memStatsUnmappedTotalUnit = ByteFormat::Format(statistics.unmapped_total, &memStatsUnmappedTotal);

        cvar camPos = Camera::GetMainCamera()->GetPosition();
        cvar camRot = Camera::GetMainCamera()->GetRotation();

        cvar previousDepth = UI::GetDepth();
        UI::SetDepth(9999.0f);
        m_horiOffset = 10.0f;

        DrawTextLine(TEXT_CONST("Statistics [press F9 to hide, press P to show/hide profiles]"), Color(0xFF0A00FF));
        MakeLineSpace(3);

        DrawTextLine(TEXT_CONST("[Engine Statistics]"), Color(0xFF0A00FF));
        DrawTextLine(Text::Format(TEXT_CONST("FPS: {0}"), m_fps), GetFPSLineColor(m_fps));
        DrawTextLine(Text::Format(TEXT_CONST("Camera Position: X:{0:.2f} Y:{1:.2f} Z:{2:.2f}"), camPos.x, camPos.y, camPos.z), Color(0xFFFFFFFF));
        DrawTextLine(Text::Format(TEXT_CONST("Camera Rotation: X:{0:.2f} Y:{1:.2f}"), camRot.x, camRot.y), Color(0xFFFFFFFF));
        MakeLineSpace(1);
        
        DrawTextLine(TEXT_CONST("[Unmanaged Memory]"), Color(0xFF0A00FF));
        DrawTextLine(Text::Format(TEXT_CONST("Cached: {0} {1}"), memStatsCached, ByteFormat::ToString(memStatsCachedUnit)), Color(0xFFFFFFFF));
        DrawTextLine(Text::Format(TEXT_CONST("Mapped: {0} {1}"), memStatsMapped, ByteFormat::ToString(memStatsMappedUnit)), Color(0xFFFFFFFF));
        DrawTextLine(Text::Format(TEXT_CONST("Mapped Total: {0} {1}"), memStatsMappedTotal, ByteFormat::ToString(memStatsMappedTotalUnit)), Color(0xFFFFFFFF));
        DrawTextLine(Text::Format(TEXT_CONST("Unmapped Total: {0} {1}"), memStatsUnmappedTotal, ByteFormat::ToString(memStatsUnmappedTotalUnit)), Color(0xFFFFFFFF));
        MakeLineSpace(1);

        DrawTextLine(TEXT_CONST("[Managed Memory]"), Color(0xFF0A00FF));
        DrawTextLine(Text::Format(TEXT_CONST("Memory usage: {0} {1}"), gcUsedSize, ByteFormat::ToString(gcUsedSizeUnit)), Color(0xFFFFFFFF));
        DrawTextLine(Text::Format(TEXT_CONST("Heap size: {0} {1}"), gcHeapSize, ByteFormat::ToString(gcHeapSizeUnit)), Color(0xFFFFFFFF));
        MakeLineSpace(2);

        DrawTextLine(TEXT_CONST("[Render Statistics]"), Color(0xFF0A00FF));
        MakeLineSpace(1);

        DrawTextLine(Text::Format(TEXT_CONST("API Calls: {0}"), renderStats.apiCallCount), Color(0xFFFFFFFF));
        DrawTextLine(Text::Format(TEXT_CONST("Draw Calls: {0}"), renderStats.drawCallCount), Color(0xFFFFFFFF));
        DrawTextLine(Text::Format(TEXT_CONST("Frame Commands: {0}"), renderStats.commandCount), Color(0xFFFFFFFF));
        DrawTextLine(Text::Format(TEXT_CONST("Vertices Drawn: {0}"), renderStats.verticesDrawn), Color(0xFFFFFFFF));
        DrawTextLine(Text::Format(TEXT_CONST("Indices Drawn: {0}"), renderStats.indicesDrawn), Color(0xFFFFFFFF));
        DrawTextLine(Text::Format(TEXT_CONST("Vertex Buffers: {0}"), renderStats.vertexBufferCount), Color(0xFFFFFFFF));
        DrawTextLine(Text::Format(TEXT_CONST("Index Buffers: {0}"), renderStats.indexBufferCount), Color(0xFFFFFFFF));
        DrawTextLine(Text::Format(TEXT_CONST("Render Buffers: {0}"), renderStats.renderBufferCount), Color(0xFFFFFFFF));
        DrawTextLine(Text::Format(TEXT_CONST("Textures: {0}"), renderStats.texture2DCount), Color(0xFFFFFFFF));
        DrawTextLine(Text::Format(TEXT_CONST("Loaded Shaders: {0}"), renderStats.shaderCount), Color(0xFFFFFFFF));
        DrawTextLine(Text::Format(TEXT_CONST("Flags: {0}"), renderStats.flags), Color(0xFFFFFFFF));
        MakeLineSpace(2);

        if(m_drawProfiles)
        {
            m_horiOffset = 400.0f;
            m_lineOffset = 10.0f;

            MakeLineSpace(4);

            DrawTextLine(TEXT_CONST("[Profiles]"), Color(0xFF0A00FF));
            MakeLineSpace(1);

            // sort by order
            sort(m_profiles.begin(), m_profiles.end(), ProfileSort);

            for (var i = static_cast<int>(m_profiles.Count()) - 1; i >= 0; i--)
            {
                rvar entry = m_profiles.At(i);
                cvar depthOffset = entry.depth * 15.0f;

                if (!entry.updated)
                    continue;

                Color color;
                if (entry.timeAvg > entry.timeoutMax && entry.timeoutMax >= 0.0f)
                    color = Color(0xFF0A00FF);
                else if (entry.timeAvg > entry.timeoutMed && entry.timeoutMed >= 0.0f)
                    color = Color(0xAA0A00FF);
                else
                    color = Color(0xFFFFFFFF);

                cvar offset = Vector2(m_horiOffset + 225.0f + depthOffset, m_lineOffset);

                if (entry.utf8)
                {
                    UI::SetColor(Color(0xFFFFFFFF));
                    cvar name = static_cast<const char*>(entry.name);
                    UI::DrawText(m_debugFont, name, static_cast<int>(strlen(name)), offset);
                }
                else
                {
                    UI::SetColor(Color(0xFFAAAAFF));
                    cvar name = static_cast<const Char*>(entry.name);
                    UI::DrawText(m_debugFont, name, Text::Length(name), offset);
                }

                DrawTextLine(Text::Format(TEXT("{0:.3f} ms  {1:.3f} ms  {2:.3f} ms"), entry.timeAvg, entry.timeMin, entry.timeMax), color);
                entry.updated = false;
            }
        }

        UI::SetDepth(previousDepth);
    }
    EndProfile();
}

void Profiler::EndFrame()
{
    // cleanup

    m_profileCount = 0;

    // clear stack if needed and start yelling at the dev
    if (m_profileStack.Count() > 0)
    {
        m_profileStack.Clear();
        Logger::LogWarning("Profiler: Not all of the profiles were ended!");
    }

    auto currentTime = Platform::GetMiliseconds();

    // Remove old profiles
    m_profiles.Erase(std::remove_if(m_profiles.begin(), m_profiles.end(), [currentTime](Profile& profile)
    {
        return currentTime - profile.lastUpdate >= 1000.0f;
    }), m_profiles.end());
}
