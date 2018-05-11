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
bool Profiler::m_drawDebugScreen;
Font* Profiler::m_debugFont;
int Profiler::m_profileCount;

float m_lastFPSCountTime = 0;
int m_frames = 0;
int m_fps = 0;

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

    if(Time::time() - m_lastFPSCountTime > 1.0f)
    {
        m_fps = m_frames;
        m_frames = 0;
        m_lastFPSCountTime = Time::time();
    }

	if (Input::isKeyDown(Key_F9))
		m_drawDebugScreen = !m_drawDebugScreen;
}

void Profiler::drawDebugScreen()
{
	if (!m_drawDebugScreen)
		return;

	beginProfile("Profile");
	endProfile();

	beginProfile("Profiler Draw (self profile)", 0.5f, 1.0f);
	{
		var depth = UI::getDepth();
		UI::setDepth(9999.0f);

		var yOffset = 0.0f;

		float gcUsedSize;
		var unitA = ByteFormat::Format(mono_gc_get_used_size(), &gcUsedSize);

		float gcHeapSize;
		var unitB = ByteFormat::Format(mono_gc_get_heap_size(), &gcHeapSize);

		UI::setColor(Color(0xFF0A00FF));
		UI::drawText(m_debugFont, TEXT_CONST("Profiler [press F9 to hide]"), Vector2(0.0f, 0.0f));
		yOffset += static_cast<float>(m_debugFont->getSize()) * 2.0f;

        UI::setColor(Color(0xFFFFFFFF));
		UI::drawText(m_debugFont, TEXT_CONST("[GC Memory]"), Vector2(0.0f, yOffset));
		yOffset += static_cast<float>(m_debugFont->getSize()) * 2.0f;

		UI::drawText(m_debugFont,
            Text::format(TEXT_CONST("FPS: {0}\nUsed memory: {1} {2}\nHeap size: {3} {4}"), m_fps, gcUsedSize, ByteFormat::ToString(unitA), gcHeapSize, ByteFormat::ToString(unitB)),
			Vector2(0.0f, yOffset)
		);
		yOffset += static_cast<float>(m_debugFont->getSize()) * 3.0f;

		UI::drawText(m_debugFont, TEXT_CONST("[Profiles]"), Vector2(0.0f, yOffset));
		yOffset += static_cast<float>(m_debugFont->getSize()) * 2.0f;

		// sort by order
		sort(m_profiles.begin(), m_profiles.end(), profileSort);

		for (var i = static_cast<int>(m_profiles.count()) - 1; i >= 0; i--)
		{
			UI::setColor(Color(0xFFFFFFFF));

			var entry = m_profiles.at(i);
			var depthOffset = entry.depth * 15.0f;

			if (entry.timeAvg > entry.timeMax && entry.timeMax >= 0.0f)
				UI::setColor(Color(0xFF0A00FF));
			else if (entry.timeAvg > entry.timeMed && entry.timeMed >= 0.0f)
				UI::setColor(Color(0xAA0A00FF));
			else
				UI::setColor(Color(0xFFFFFFFF));

			var infoText = Text::format(TEXT("{0:.4f} ms"), entry.timeAvg);

			UI::drawText(m_debugFont, infoText, Vector2(10.0f, yOffset));

			if(entry.utf8)
			{
				var name = static_cast<const char*>(entry.name);
				UI::drawText(m_debugFont, name, static_cast<int>(strlen(name)), Vector2(150.0f + depthOffset, yOffset));
			}
			else
			{
				var name = static_cast<const Char*>(entry.name);
				UI::drawText(m_debugFont, name, Text::length(name), Vector2(150.0f + depthOffset, yOffset));
			}

			yOffset += m_debugFont->getSize();
		}

		UI::setDepth(depth);
	}
	endProfile();
}

void Profiler::endFrame()
{
	// cleanup

	m_profileCount = 0;
	
	// clear stack if needed and start yelling at the dev
	if(m_profileStack.count() > 0)
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
