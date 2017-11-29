// ReCrafted © 2016-2017 Always Too Late

#pragma once

#ifndef PROFILER_H
#define PROFILER_H

// includes
#include "ReCrafted.h"
#include "Platform/Platform.h"
#include "Core/Containers/Array.h"

class Font;

/// <summary>
/// Profiler class.
/// </summary>
class Profiler
{
	friend class GameMain;

private:
	API_DEF

private:
	struct ProfilerEntry
	{
	public:
		double time = 0.0f;
		double timeMed = 0.0f;
		double timeMax = 0.0f;
		Text name = {};

		int depth = 0;
	};

private:
	static Array<ProfilerEntry> m_entries;
	static Array<ProfilerEntry> m_entryiesBuffer;
	static Array<ProfilerEntry> m_lastFrame;
	static bool m_drawDebugScreen;
	static Font* m_debugFont;

private:
	static void init();
	static void update();
	static void drawDebugScreen();
	static void endFrame();

public:
	/**
	 * \brief Begins new profile.
	 * \param name The name of the new profile. Use `TEXT_CONST("Text")`.
	 */
	FORCEINLINE static void beginProfile(Text name, double timeMed = 17.0f, double timeMax = 17.0f)
	{
		auto start = Platform::getMiliseconds();

		ProfilerEntry entry;
		entry.time = start;
		entry.name = name;
		entry.timeMed = timeMed;
		entry.timeMax = timeMax;
		entry.depth = m_entries.count();

		m_entries.add(entry);
	}

	/**
	* \brief Ends the current profile.
	*/
	FORCEINLINE static void endProfile()
	{
		var entry = m_entries.last();

		auto start = entry.time;
		auto end = Platform::getMiliseconds();
		auto time = end - start;

		entry.time = time;
		m_entryiesBuffer.add(entry);
		m_entries.removeAt(m_entries.count() - 1);
	}

	/**
	 * \brief Gets the last frame info.
	 * \return The array which contains all of the profiles.
	 */
	FORCEINLINE static Array<ProfilerEntry> getFrameInfo()
	{
		return m_lastFrame;
	}
};

#endif // PROFILER_H
