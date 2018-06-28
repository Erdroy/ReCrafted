// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef PROFILER_H
#define PROFILER_H

// includes
#include "ReCrafted.h"
#include "Core/EngineComponent.h"
#include "Platform/Platform.h"
#include "Core/Containers/Array.h"
#include "Core/Math/Color.h"
#include "Core/Math/Math.h"

class Font;

/// <summary>
/// Profiler class.
/// </summary>
class Profiler : public EngineComponent<Profiler>
{
    friend class EngineMain;
    friend class Graphics;

private:
SCRIPTING_API_IMPL()

private:
    struct Profile
    {
    public:
        float timeTotal = 0.0f;
        float timeAvg = 0.0f;
        float timeMin = FLT_MAX;
        float timeMax = 0.0f;

        float timeoutMed = 0.0f;
        float timeoutMax = 0.0f;

        double lastUpdate = 0.0f;
        double lastAvgUpdate = 0.0f;
        double timeBegin = 0.0f;

        int order = 0;
        int calls = 0;
        int depth = 0;

        const void* name = nullptr;
        bool utf8 = false;

        bool updated = false;

    public:
        void Update(double currentTime)
        {
            // Update time
            lastUpdate = currentTime;
            timeBegin = currentTime;
            depth = m_profileStack.Count();
            order = m_profileCount;

            // increment profile count
            m_profileCount++;

            updated = true;

            // increment call count
            calls++;
        }
    };

private:
    static Array<Profile> m_profiles;
    static Array<Profile*> m_profileStack;
    static int m_profileCount;

    bool m_drawDebugScreen = false;
    bool m_drawProfiles = false;
    Font* m_debugFont = nullptr;
    float m_lineOffset = 0.0f;
    float m_horiOffset = 10.0f;
    float m_lastFPSCountTime = 0;
    int m_frames = 0;
    int m_fps = 0;

public:
    virtual ~Profiler() = default;

private:
    static bool ProfileSort(const Profile& lhs, const Profile& rhs);

    void OnInit() override;
    void OnDispose() override;
    void Update() override;

    void DrawTextLine(Text text, Color color);
    void MakeLineSpace(int lines);

    void DrawDebugScreen();

public:
    void EndFrame();

    Font* GetDebugFont() const
    {
        return m_debugFont;
    }

private:
    template <typename T>
    FORCEINLINE static void BeginProfile(const T* name, bool utf8, float timeMed, float timeMax)
    {
        // TODO: Main thread check (debug)

        cvar currentTime = Platform::GetMiliseconds();

        // try select profile, then Update
        // check if profile already exists with this name

        if (m_profiles.Count() > 0)
        {
            for (var&& profile : m_profiles)
            {
                if (utf8)
                {
                    // (just Compare name pointers, not called by Mono, 
                    // so 'const char*' pointer address is const...)
                    if (profile.name == name)
                    {
                        // Update
                        profile.Update(currentTime);

                        // add to stack
                        m_profileStack.Add(&profile);
                        return;
                    }
                }
                else
                {
                    // TODO: UTF-16 string Compare
                    if (Text::Compare(static_cast<const Char*>(profile.name), static_cast<const Char*>((void*)name)))
                    {
                        // Update
                        profile.Update(currentTime);

                        // add to stack
                        m_profileStack.Add(&profile);
                        return;
                    }
                }
            }
        }

        // add profile as it is not yet added
        Profile newProfile;
        newProfile.name = name;
        newProfile.calls = 0;
        newProfile.timeoutMed = timeMed;
        newProfile.timeoutMax = timeMax;
        newProfile.utf8 = utf8;

        newProfile.Update(currentTime);

        m_profiles.Add(newProfile);
    }

public:
    /**
	 * \brief Begins new profile.
	 * \param name The name of the new profile. Use `TEXT_CHARS("Text")`.
	 */
    FORCEINLINE static void BeginProfile(const Char* name, float timeMed = -1.0f, float timeMax = -1.0f)
    {
        BeginProfile(name, false, timeMed, timeMax);
    }

    /**
    * \brief Begins new profile.
    * \param name The name of the new profile.
    */
    FORCEINLINE static void BeginProfile(const char* name, float timeMed = -1.0f, float timeMax = -1.0f)
    {
        BeginProfile(name, true, timeMed, timeMax);
    }

    /**
    * \brief Ends the current profile.
    */
    FORCEINLINE static void EndProfile()
    {
        if (m_profileStack.Count() == 0)
            return;

        cvar currentTime = Platform::GetMiliseconds();

        // select profile
        crvar profile = m_profileStack.Last();

        cvar time = currentTime - profile->timeBegin;

        // Update total time
        profile->timeTotal += static_cast<float>(time);

        profile->timeMin = Math::MinF(float(time), profile->timeMin);
        profile->timeMax = Math::MaxF(float(time), profile->timeMax);

        // Update every second
        if (currentTime - profile->lastAvgUpdate >= 1000.0f)
        {
            // calculate avg time
            profile->timeAvg = profile->timeTotal / float(profile->calls);
            profile->lastAvgUpdate = currentTime;

            profile->timeMin = float(time);
            profile->timeMax = float(time);

            profile->timeTotal = 0.0f;
            profile->calls = 1;
        }

        // Remove profile
        m_profileStack.RemoveAt(m_profileStack.Count() - 1);
    }
};

#endif // PROFILER_H
