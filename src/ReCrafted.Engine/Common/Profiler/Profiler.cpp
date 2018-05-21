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

bool Profiler::profileSort(const Profile& lhs, const Profile& rhs)
{
    return lhs.order > rhs.order;
}

void Profiler::onInit()
{
    var debugFont = TEXT_CONST("../assets/fonts/VeraMono.ttf");

    m_drawDebugScreen = false;

    m_debugFont = new Font();
    m_debugFont->loadFont(debugFont, 12);
}

void Profiler::onDispose()
{
    // destroy font
    Object::destroy(m_debugFont);
}

void Profiler::update()
{
    m_frames++;
    m_lineOffset = 10.0f;

    if (Time::time() - m_lastFPSCountTime > 1.0f)
    {
        m_fps = m_frames;
        m_frames = 0;
        m_lastFPSCountTime = Time::time();
    }

    if (Input::isKeyDown(Key_F9))
        m_drawDebugScreen = !m_drawDebugScreen;
}

void Profiler::drawLine(Text text, Color color)
{
    UI::setColor(color);
    UI::drawText(m_debugFont, text, Vector2(10.0f, m_lineOffset));
    m_lineOffset += static_cast<float>(m_debugFont->getSize());
}

void Profiler::space(int lines)
{
    m_lineOffset += static_cast<float>(m_debugFont->getSize()) * lines;
}

void Profiler::drawDebugScreen()
{
    if (!m_drawDebugScreen)
        return;

    beginProfile("Profile");
    endProfile();

    beginProfile("Profiler Draw (self profile)", 0.5f, 1.0f);
    {
        Renderer::RenderStatistics renderStats = {};
        Renderer::GetRenderStatistics(&renderStats);

        cvar previousDepth = UI::getDepth();
        UI::setDepth(9999.0f);

        drawLine(TEXT_CONST("Profiler [press F9 to hide]"), Color(0xFF0A00FF));
        space(3);

        drawLine(TEXT_CONST("[Engine Statistics]"), Color(0xFF0A00FF));
        space(1);

        float gcUsedSize;
        float gcHeapSize;
        cvar unitA = ByteFormat::Format(mono_gc_get_used_size(), &gcUsedSize);
        cvar unitB = ByteFormat::Format(mono_gc_get_heap_size(), &gcHeapSize);
        drawLine(Text::format(TEXT_CONST("FPS: {0}"), m_fps), Color(0xFFFF00FF));
        drawLine(Text::format(TEXT_CONST("GC memory usage: {0} {1}"), gcUsedSize, ByteFormat::ToString(unitA)), Color(0xFFFFFFFF));
        drawLine(Text::format(TEXT_CONST("GC Heap size: {0} {1}"), gcHeapSize, ByteFormat::ToString(unitB)), Color(0xFFFFFFFF));
        space(2);

        drawLine(TEXT_CONST("[Render Statistics]"), Color(0xFF0A00FF));
        space(1);

        drawLine(Text::format(TEXT_CONST("API Calls: {0}"), renderStats.apiCallCount), Color(0xFFFFFFFF));
        drawLine(Text::format(TEXT_CONST("Draw Calls: {0}"), renderStats.drawCallCount), Color(0xFFFFFFFF));
        drawLine(Text::format(TEXT_CONST("Frame Commands: {0}"), renderStats.commandCount), Color(0xFFFFFFFF));
        drawLine(Text::format(TEXT_CONST("Vertices Drawn: {0}"), renderStats.verticesDrawn), Color(0xFFFFFFFF));
        drawLine(Text::format(TEXT_CONST("Indices Drawn: {0}"), renderStats.indicesDrawn), Color(0xFFFFFFFF));
        drawLine(Text::format(TEXT_CONST("Vertex Buffers: {0}"), renderStats.vertexBufferCount), Color(0xFFFFFFFF));
        drawLine(Text::format(TEXT_CONST("Index Buffers: {0}"), renderStats.indexBufferCount), Color(0xFFFFFFFF));
        drawLine(Text::format(TEXT_CONST("Render Buffers: {0}"), renderStats.renderBufferCount), Color(0xFFFFFFFF));
        drawLine(Text::format(TEXT_CONST("Textures: {0}"), renderStats.texture2DCount), Color(0xFFFFFFFF));
        drawLine(Text::format(TEXT_CONST("Loaded Shaders: {0}"), renderStats.shaderCount), Color(0xFFFFFFFF));
        drawLine(Text::format(TEXT_CONST("Flags: {0}"), renderStats.flags), Color(0xFFFFFFFF));
        space(2);

        drawLine(TEXT_CONST("[Profiles]"), Color(0xFF0A00FF));
        space(1);

        // sort by order
        sort(m_profiles.begin(), m_profiles.end(), profileSort);

        for (var i = static_cast<int>(m_profiles.count()) - 1; i >= 0; i--)
        {
            rvar entry = m_profiles.at(i);
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
                UI::setColor(Color(0xFFFFFFFF));
                cvar name = static_cast<const char*>(entry.name);
                UI::drawText(m_debugFont, name, static_cast<int>(strlen(name)), offset);
            }
            else
            {
                UI::setColor(Color(0xFFAAAAFF));
                cvar name = static_cast<const Char*>(entry.name);
                UI::drawText(m_debugFont, name, Text::length(name), offset);
            }

            drawLine(Text::format(TEXT("{0:.4f} ms"), entry.timeAvg), color);
            entry.updated = false;
        }

        UI::setDepth(previousDepth);
    }
    endProfile();
}

void Profiler::endFrame()
{
    // cleanup

    m_profileCount = 0;

    // clear stack if needed and start yelling at the dev
    if (m_profileStack.count() > 0)
    {
        m_profileStack.clear();
        Logger::logWarning("Profiler: Not all of the profiles were ended!");
    }

    auto currentTime = Platform::getMiliseconds();

    // remove old profiles
    m_profiles.erase(std::remove_if(m_profiles.begin(), m_profiles.end(), [currentTime](Profile& profile)
    {
        return currentTime - profile.lastUpdate >= 1000.0f;
    }), m_profiles.end());
}
