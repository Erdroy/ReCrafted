// ReCrafted © 2016-2017 Always Too Late

#include "Profiler.h"
#include "Core/Logger.h"
#include "Common/Input.h"
#include "Graphics/UI/UI.h"
#include "Graphics/Font.h"
#include <iomanip>
#include <sstream>

Array<Profiler::ProfilerEntry> Profiler::m_entries;
Array<Profiler::ProfilerEntry> Profiler::m_entryiesBuffer;
Array<Profiler::ProfilerEntry> Profiler::m_lastFrame;
bool Profiler::m_drawDebugScreen;
Font* Profiler::m_debugFont;

template <typename T>
std::wstring to_string_with_precision(const T a_value, const int n)
{
	std::wostringstream out;
	out << std::fixed << std::setprecision(n) << a_value;
	return out.str();
}

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
		UI::setColor(Color(0xFFFFFFFF));

		var entry = m_lastFrame.at(i);

		var depthOffset = entry.depth * 15.0f;
		var timeStr = to_string_with_precision(entry.time, 4);
		var time = Text::constant((Char*)timeStr.data());

		if (entry.time > entry.timeMax && entry.timeMax >= 0.0f)
			UI::setColor(Color(0xFF0A00FF));
		else if (entry.time > entry.timeMed && entry.timeMed >= 0.0f)
			UI::setColor(Color(0xAA0A00FF));
		else
			UI::setColor(Color(0xFFFFFFFF));

		var timeSize = m_debugFont->measureText(time);

		UI::drawText(m_debugFont, time, Vector2(10.0f, yOffset));
		UI::drawText(m_debugFont, TEXT_CONST("ms"), Vector2(timeSize.x + 15.0f, yOffset));

		UI::drawText(m_debugFont, entry.name, Vector2(timeSize.x + 35.0f + depthOffset, yOffset));

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
