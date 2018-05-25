// ReCrafted (c) 2016-2018 Always Too Late

#include "Profiler.h"
#include "Common/Time.h"
#include "Common/Input/Input.h"
#include "Core/Logger.h"
#include "Core/ByteFormat.h"
#include "Graphics/UI/UI.h"
#include "Graphics/Font.h"

#include <mono/metadata/mono-gc.h>

SINGLETON_IMPL(Profiler)

Array<Profiler::Profile> Profiler::m_profiles;
Array<Profiler::Profile*> Profiler::m_profileStack;
int Profiler::m_profileCount;

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
}

void Profiler::DrawTextLine(Text text, Color color)
{
    UI::SetColor(color);
    UI::DrawText(m_debugFont, text, Vector2(10.0f, m_lineOffset));
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

        cvar previousDepth = UI::GetDepth();
        UI::SetDepth(9999.0f);

        DrawTextLine(TEXT_CONST("Profiler [press F9 to hide]"), Color(0xFF0A00FF));
        MakeLineSpace(3);

        DrawTextLine(TEXT_CONST("[Engine Statistics]"), Color(0xFF0A00FF));
        MakeLineSpace(1);

        float gcUsedSize;
        float gcHeapSize;
        cvar unitA = ByteFormat::Format(mono_gc_get_used_size(), &gcUsedSize);
        cvar unitB = ByteFormat::Format(mono_gc_get_heap_size(), &gcHeapSize);
        DrawTextLine(Text::Format(TEXT_CONST("FPS: {0}"), m_fps), Color(0xFFFF00FF));
        DrawTextLine(Text::Format(TEXT_CONST("GC memory usage: {0} {1}"), gcUsedSize, ByteFormat::ToString(unitA)), Color(0xFFFFFFFF));
        DrawTextLine(Text::Format(TEXT_CONST("GC Heap size: {0} {1}"), gcHeapSize, ByteFormat::ToString(unitB)), Color(0xFFFFFFFF));
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
            if (entry.timeAvg > entry.timeMax && entry.timeMax >= 0.0f)
                color = Color(0xFF0A00FF);
            else if (entry.timeAvg > entry.timeMed && entry.timeMed >= 0.0f)
                color = Color(0xAA0A00FF);
            else
                color = Color(0xFFFFFFFF);

            cvar offset = Vector2(150.0f + depthOffset, m_lineOffset);

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

            DrawTextLine(Text::Format(TEXT("{0:.4f} ms"), entry.timeAvg), color);
            entry.updated = false;
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
