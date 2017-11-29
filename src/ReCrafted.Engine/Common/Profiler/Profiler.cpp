// ReCrafted © 2016-2017 Always Too Late

#include "Profiler.h"
#include "Core/Logger.h"
#include "Common/Input.h"
#include "Graphics/UI/UI.h"
#include "Graphics/Font.h"

Array<Profiler::ProfilerEntry> Profiler::m_entries;
Array<Profiler::ProfilerEntry> Profiler::m_entryiesBuffer;
Array<Profiler::ProfilerEntry> Profiler::m_lastFrame;
bool Profiler::m_drawDebugScreen;
Font* Profiler::m_debugFont;

void Profiler::init()
{
	m_drawDebugScreen = false;

	m_debugFont = new Font();
	m_debugFont->loadFont(TEXT_CONST("../assets/fonts/VeraMono.ttf"), 12);
}

void Profiler::update()
{
	if (Input::isKeyDown(Key_F9))
		m_drawDebugScreen = !m_drawDebugScreen;
}

void Profiler::drawDebugScreen()
{
	if (!m_drawDebugScreen)
		return;

	var yOffset = static_cast<float>(m_debugFont->getSize());

	UI::drawText(m_debugFont, TEXT_CONST("Profiler [press F9 to hide]"), Vector2(0.0f, 0.0f));

	for (var i = static_cast<int>(m_lastFrame.count()) - 1; i >= 0; i --)
	{
		var entry = m_lastFrame.at(i);

		var timeStr = std::to_wstring(entry.time);
		var time = Text::constant((Char*)timeStr.data());

		var nameSize = 170.0f;
		var timeSize = m_debugFont->measureText(time);

		UI::drawText(m_debugFont, entry.name, Vector2(0.0f, yOffset));
		UI::drawText(m_debugFont, time, Vector2(nameSize + 10.0f, yOffset));
		UI::drawText(m_debugFont, TEXT_CONST("ms"), Vector2(nameSize + timeSize.x + 15.0f, yOffset));

		yOffset += m_debugFont->getSize();
	}
}

void Profiler::endFrame()
{
	// copy to present array
	m_lastFrame.clear();
	m_lastFrame.copy(m_entryiesBuffer);
	m_entryiesBuffer.clear();

	if(m_entries.count() != 0)
	{
		Logger::logWarning("Profiler: Not all of the profiles were ended!");
		m_entries.clear();
	}
}
