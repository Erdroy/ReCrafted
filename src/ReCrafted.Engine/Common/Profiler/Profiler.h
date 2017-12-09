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
	struct Profile
	{
	public:
		float timeTotal = 0.0f;
		float timeAvg = 0.0f;

		float timeMed = 0.0f;
		float timeMax = 0.0f;

		double lastUpdate = 0.0f;
		double lastAvgUpdate = 0.0f;
		double timeBegin = 0.0f;

		int calls = 0;
		int depth = 0;

		const void* name = nullptr;
		bool utf8 = false;

	public:
		void update(double currentTime)
		{
			// update time
			lastUpdate = currentTime;
			timeBegin = currentTime;
			depth = m_profileStack.count();

			// increment call count
			calls++;
		}
	};

private:
	static Array<Profile> m_profiles;
	static Array<Profile*> m_profileStack;
	static bool m_drawDebugScreen;
	static Font* m_debugFont;

private:
	static void init();
	static void update();
	static void drawDebugScreen();

private:
	template<typename T>
	FORCEINLINE static void beginProfile(const T* name, bool utf8, float timeMed, float timeMax)
	{
		// TODO: Main thread check (debug)

		var currentTime = Platform::getMiliseconds();

		// try select profile, then update
		// check if profile already exists with this name
		for (var & profile : m_profiles)
		{
			if (utf8)
			{
				// (just compare name pointers, not called by Mono, 
				// so 'const char*' pointer address is const...)
				if (profile.name == name)
				{
					// update
					profile.update(currentTime);

					// add to stack
					m_profileStack.add(&profile);
					return;
				}
			}
			else
			{
				// TODO: UTF-16 string compare
				return;
			}
		}

		// add profile as it is not yet added
		Profile newProfile;
		newProfile.name = name;
		newProfile.calls = 0;
		newProfile.depth = m_profileStack.count();
		newProfile.timeMed = timeMed;
		newProfile.timeMax = timeMax;
		newProfile.lastUpdate = currentTime;
		newProfile.timeBegin = currentTime;
		newProfile.utf8 = utf8;

		m_profiles.add(newProfile);
	}

public:
	/**
	 * \brief Begins new profile.
	 * \param name The name of the new profile. Use `TEXT_CHARS("Text")`.
	 */
	FORCEINLINE static void beginProfile(const Char* name, float timeMed = -1.0f, float timeMax = -1.0f)
	{
		beginProfile(name, false, timeMed, timeMax);
	}

	/**
	* \brief Begins new profile.
	* \param name The name of the new profile.
	*/
	FORCEINLINE static void beginProfile(const char* name, float timeMed = -1.0f, float timeMax = -1.0f) 
	{
		beginProfile(name, true, timeMed, timeMax);
	}

	/**
	* \brief Ends the current profile.
	*/
	FORCEINLINE static void endProfile() 
	{
		if (m_profileStack.count() == 0)
			return;

		var currentTime = Platform::getMiliseconds();

		// select profile
		var profile = m_profileStack.last();

		// update total time
		profile->timeTotal += static_cast<float>(currentTime - profile->timeBegin);

		// update every 1/4 second
		if ((currentTime - profile->lastAvgUpdate) * 0.001f >= 0.25f)
		{
			// calculate avg time
			profile->timeAvg = profile->timeTotal / float(profile->calls);
			profile->lastAvgUpdate = currentTime;
		}

		// remove profile
		m_profileStack.removeAt(m_profileStack.count() - 1);
	}

	/**
	 * \brief Ends current frame.
	 */
	static void endFrame();
};

#endif // PROFILER_H
