// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef PROFILER_H
#define PROFILER_H

#include "ReCrafted.h"
#include "Core/EngineComponent.h"

#include "spp.h"
#include <atomic>
#include <queue>

class Profiler : public EngineComponent<Profiler>
{
public:
    static const int ProfileSeconds = 4; // TODO: This should be configurable

private:
    struct ProfileEntry
    {
        std::string profileName{};
        double startTime_ms = 0.0;
        double endTime_ms = 0.0;
        float profileTime_ms = 0.0f;
    };

    struct ProfileFrame
    {
        double startTime_ms = 0.0;
        double endTime_ms = 0.0;
        float time_ms = 0.0f;
        std::vector<ProfileEntry> profiles;
        std::queue<ProfileEntry> profileQueue;
    };

    class ThreadData
    {
    public:
        std::thread::id threadId;
        const char* threadName = nullptr;
        std::vector<ProfileFrame> frames = {};

        ProfileFrame currentFrame;

    public:
        void BeginFrame();
        void PushFrame();
        void EndFrame();

    public:
        uint32_t BeginCPUProfile(const char* name);
        void EndCPUProfile(uint32_t profileId);
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

private:
    void DrawWindow();
    void DrawThread(ThreadData* thread);

protected:
    void OnInit() override;
    void OnDispose() override;
    void Update() override;

public:
    static void InitThread(const char* threadName);
    static void FinishThread();

    static void BeginFrame();
    static void PushFrame();
    static void EndFrame(bool autoPush = true);

public:
    FORCEINLINE static uint32_t BeginCPUProfile(const char* name)
    {
        ASSERT(GetInstance());

        if (!GetInstance()->m_profilingEnabled)
            return 0u;  
        
        cvar threadData = GetInstance()->GetCurrentThreadData();
        _ASSERT_(threadData, "Profiler thread not found. Are you sure that you've called Profiler::InitThread?");

        // Begin new CPU profile
        return threadData->BeginCPUProfile(name);
    }

    FORCEINLINE static void EndCPUProfile(const uint32_t profileId = 0u)
    {
        ASSERT(GetInstance());

        if (GetInstance()->m_profilingEnabled)
        {
            cvar threadData = GetInstance()->GetCurrentThreadData();
            _ASSERT_(threadData, "Profiler thread not found. Are you sure that you've called Profiler::InitThread?");

            // End current CPU profile
            threadData->EndCPUProfile(profileId);
        }
    }

public:
    FORCEINLINE static void BeginProfile(const char* name)
    {
        BeginCPUProfile(name);
    }

    FORCEINLINE static void EndProfile()
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

#define BEGIN_CPU_PROFILE(name) Profiler::BeginCPUProfile(name);
#define END_CPU_PROFILE(name) Profiler::EndCPUProfile();
#define CPU_SCOPE_PROFILE(name) ScopedProfile(name); 

#endif // PROFILER_H
