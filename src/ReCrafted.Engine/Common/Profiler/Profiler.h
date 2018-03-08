// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef PROFILER_H
#define PROFILER_H

// includes
#include "ReCrafted.h"
#include "Core/EngineComponent.h"
#include "Platform/Platform.h"
#include "Core/Containers/Array.h"

class Font;

/// <summary>
/// Profiler class.
/// </summary>
class Profiler : public EngineComponent<Profiler>
{
	friend class EngineMain;
	friend class Renderer;

private:
	SCRIPTING_API_IMPL()

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

		int order = 0;
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
			order = m_profileCount;

			// increment profile count
			m_profileCount++;

			// increment call count
			calls++;
		}
	};

private:
	static Array<Profile> m_profiles;
	static Array<Profile*> m_profileStack;
	static bool m_drawDebugScreen;
	static Font* m_debugFont;
	static int m_profileCount;

private:
	static bool Profiler::profileSort(const Profile& lhs, const Profile& rhs);

private:
    void onInit() override;
    void onDispose() override;

private:
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
		
	    if(m_profiles.count() > 0)
	    {
            for (var && profile : m_profiles)
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
                    if (Text::compare(static_cast<const Char*>(profile.name), static_cast<const Char*>((void*)name)))
                    {
                        // update
                        profile.update(currentTime);

                        // add to stack
                        m_profileStack.add(&profile);
                        return;
                    }
                }
            }
	    }

		// add profile as it is not yet added
		Profile newProfile;
		newProfile.name = name;
		newProfile.calls = 0;
		newProfile.timeMed = timeMed;
		newProfile.timeMax = timeMax;
		newProfile.utf8 = utf8;

		newProfile.update(currentTime);

		m_profiles.add(newProfile);
	}

public:
    virtual ~Profiler() = default;

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
		if (currentTime - profile->lastAvgUpdate >= 250.0f)
		{
			// calculate avg time
			profile->timeAvg = profile->timeTotal / float(profile->calls);
			profile->lastAvgUpdate = currentTime;

			profile->timeTotal = 0.0f;
			profile->calls = 1;
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
