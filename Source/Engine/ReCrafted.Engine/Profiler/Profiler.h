// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Common/Lock.h"
#include "Common/List.h"
#include "Core/SubSystems/SubSystem.h"

#include <sparsepp/spp.h>
#include <atomic>
#include <queue>

/// <summary>
///     Profiler class. 
///     Provides thread CPU and Unmanaged/Manager memory profiling functionality.
/// </summary>
API_CLASS(public, static, partial, noinherit, customNamespace="ReCrafted.API.Core")
class Profiler final : public SubSystem<Profiler>
{
    API_CLASS_BODY()

public:
    static const int NumProfiledFrames = 300; // TODO: This should be configurable

private:
    struct ProfileEntry
    {
        std::string profileName;
        double startTime_ms = 0.0;
        double endTime_ms = 0.0;
        float profileTime_ms = 0.0f;
        int depth = 0;
    };

    struct ProfileTreeEntry
    {
        ProfileTreeEntry* parent = nullptr;

        std::string name;
        float time = 0.0f;
        int callNum = 0;
        int depth = 0;

        List<ProfileTreeEntry> children;
    };

    struct ProfileFrame
    {
        double startTime_ms = 0.0;
        double endTime_ms = 0.0;
        float time_ms = 0.0f;
        std::vector<ProfileEntry> profiles;
        std::deque<ProfileEntry> profileQueue;
    };

    class ThreadData
    {
    public:
        Lock dataLock;
        std::thread::id threadId;
        const char* threadName = nullptr;
        std::vector<ProfileFrame> frames = {};

        ProfileFrame currentFrame;

        bool opened = false;
        int selectedFrame = -1;

    public:
        void BeginFrame();
        void PushFrame();
        void EndFrame();

    public:
        uint32_t BeginCPUProfile(const char* name);
        void EndCPUProfile(uint32_t profileId);
        void Cleanup();
    };

private:
    bool m_showWindow = false;
    std::vector<ThreadData*> m_threads;
    spp::sparse_hash_map<std::thread::id, ThreadData*> m_threadMap;
    std::atomic<bool> m_profilingEnabled = true;
    bool m_stopProfiling = false;
    bool m_startProfiling = false;

private:
    ThreadData* GetCurrentThreadData();
    void CompileProfiles(const std::vector<ProfileEntry>& profiles, List<ProfileTreeEntry>& treeProfiles);

private:
    void DrawWindow();
    void DrawThreadProfiles(ThreadData* thread);
    void DrawThreadProfile(const ProfileTreeEntry& event);

protected:
    void Initialize() override;
    void Shutdown() override;
    void OnUpdate() override;

public:
    void ToggleProfiling();

public:
    static void InitThread(const char* threadName);
    static void FinishThread();

    static void BeginFrame();
    static void PushFrame();
    static void EndFrame(bool autoPush = true);

public:
    /// <summary>
    ///     Starts new CPU profile with given name.
    /// </summary>
    /// <returns>The current profile ID, use it to end the CPU profile.</returns>
    /// <remarks>To end this profile, use EndCPUProfile(...) function.</remarks>
    API_FUNCTION()
    FORCE_INLINE static uint32_t BeginCPUProfile(const char* name)
    {
        ASSERT(GetInstance());

        if (!GetInstance()->m_profilingEnabled)
            return 0u;

        const auto threadData = GetInstance()->GetCurrentThreadData();
        _ASSERT_(threadData, "Profiler thread not found. Are you sure that you've called Profiler::InitThread?");

        // Begin new CPU profile
        return threadData->BeginCPUProfile(name);
    }

    /// <summary>
    ///     Ends CPU profile.
    /// </summary>
    API_FUNCTION()
    FORCE_INLINE static void EndCPUProfile(const uint32_t profileId = 0)
    {
        ASSERT(GetInstance());

        if (GetInstance()->m_profilingEnabled)
        {
            const auto threadData = GetInstance()->GetCurrentThreadData();
            _ASSERT_(threadData, "Profiler thread not found. Are you sure that you've called Profiler::InitThread?");

            // End current CPU profile
            threadData->EndCPUProfile(profileId);
        }
    }

public:
    /// <summary>
    ///     Starts new CPU profile with given name.
    /// </summary>
    API_FUNCTION()
    FORCE_INLINE static void BeginProfile(const char* name)
    {
        BeginCPUProfile(name);
    }

    /// <summary>
    ///     Ends current CPU profile.
    /// </summary>
    API_FUNCTION()
    FORCE_INLINE static void EndProfile()
    {
        EndCPUProfile();
    }
};

struct ScopedProfile
{
private:
    uint32_t m_id = 0u;

private:
    ScopedProfile() = default;

public:
    explicit ScopedProfile(const char* name)
    {
        m_id = Profiler::BeginCPUProfile(name);
    }

    ~ScopedProfile()
    {
        Profiler::EndCPUProfile(m_id);
    }
};

#define CPU_PROFILE_SCOPE(id, name) const auto profile##id = ScopedProfile( name )
#define CPU_PROFILE_FUNCTION(id) const auto profile##id = ScopedProfile( __FUNCTION__ )