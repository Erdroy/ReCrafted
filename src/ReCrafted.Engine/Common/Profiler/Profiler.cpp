// ReCrafted © 2016-2017 Always Too Late

#include "Profiler.h"
#include "Core/Logger.h"
#include "Core/ByteFormat.h"
#include "Common/Input/Input.h"
#include "Graphics/UI/UI.h"
#include "Graphics/Font.h"

#include <mono/metadata/mono-gc.h>

Array<Profiler::ProfilerEntry> Profiler::m_entries;
Array<Profiler::ProfilerEntry> Profiler::m_entryiesBuffer;
Array<Profiler::ProfilerEntry> Profiler::m_lastFrame;
bool Profiler::m_drawDebugScreen;
Font* Profiler::m_debugFont;

struct BufferItem
{
	Profiler::ProfilerEntry entry;
};

Array<BufferItem> m_drawBuffer;

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

	beginProfile("Profiler Draw (self profile)", 0.5f, 1.0f);
	{
		var depth = UI::getDepth();
		UI::setDepth(9999.0f);

		recalcAvg();

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
			Text::format(TEXT("Used size: {0} {1}\nHeap size: {2} {3}"),
				gcUsedSize, ByteFormat::ToString(unitA), gcHeapSize, ByteFormat::ToString(unitB)),
			Vector2(0.0f, yOffset)
		);
		yOffset += static_cast<float>(m_debugFont->getSize()) * 3.0f;

		UI::drawText(m_debugFont, TEXT_CONST("[Profiles]"), Vector2(0.0f, yOffset));
		yOffset += static_cast<float>(m_debugFont->getSize()) * 2.0f;

		for (var i = static_cast<int>(m_lastFrame.count()) - 1; i >= 0; i--)
		{
			UI::setColor(Color(0xFFFFFFFF));

			var entry = m_lastFrame.at(i);
			var depthOffset = entry.depth * 15.0f;

			if (entry.time > entry.timeMax && entry.timeMax >= 0.0f)
				UI::setColor(Color(0xFF0A00FF));
			else if (entry.time > entry.timeMed && entry.timeMed >= 0.0f)
				UI::setColor(Color(0xAA0A00FF));
			else
				UI::setColor(Color(0xFFFFFFFF));

			var infoText = Text::format(TEXT("{0:.4f} ms"), entry.time);

			UI::drawText(m_debugFont, infoText, Vector2(10.0f, yOffset));

			if(entry.nameU8 != nullptr)
				UI::drawText(m_debugFont, entry.nameU8, static_cast<int>(strlen(entry.nameU8)), Vector2(150.0f + depthOffset, yOffset));
			else
				UI::drawText(m_debugFont, entry.name, Text::length(entry.name), Vector2(150.0f + depthOffset, yOffset));

			yOffset += m_debugFont->getSize();
		}

		UI::setDepth(depth);
	}
	endProfile();
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

void Profiler::recalcAvg()
{
	// TODO: update buffer and calculate avg
	// delete all entries that are older than one second
	// add all the current
	// calulate avg
}
