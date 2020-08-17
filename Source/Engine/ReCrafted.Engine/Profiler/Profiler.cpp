// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#include "Profiler.h"
#include "Common/Platform/Environment.h"
#include "Common/Logger.h"
#include "Input/InputManager.h"


Profiler::ThreadData* Profiler::GetCurrentThreadData()
{
    const auto it = m_threadMap.find(std::this_thread::get_id());
    if (it != m_threadMap.end())
        return it->second;
    return nullptr;
}

void Profiler::CompileProfiles(const std::vector<ProfileEntry>& profiles, List<ProfileTreeEntry>& treeProfiles)
{
    auto sortedProfiles = profiles;
    std::reverse(sortedProfiles.begin(), sortedProfiles.end());

    ProfileTreeEntry* parent = nullptr;
    ProfileTreeEntry* last = nullptr;

    for (auto&& profile : sortedProfiles)
    {
        if (last)
        {
            // Collapse same profiles
            if (profile.profileName == last->name && profile.depth == last->depth)
            {
                last->callNum++;
                last->time += profile.profileTime_ms;
                continue;
            }
        }

        // Construct entry
        ProfileTreeEntry entry;
        entry.name = profile.profileName;
        entry.time = profile.profileTime_ms;
        entry.depth = profile.depth;
        entry.callNum = 1;

        // Reset parent if depth is zero
        if (entry.depth == 0)
            parent = nullptr;

        if (last)
        {
            // Push
            if (entry.depth > last->depth)
            {
                parent = last;
            }

            // Pop
            if (entry.depth < last->depth)
            {
                if (parent)
                {
                    while (parent && parent->depth + 1 != entry.depth)
                    {
                        parent = parent->parent;
                    }
                }
            }
        }

        if (parent)
        {
            // Set parent
            entry.parent = parent;

            // Push new entry to parent
            parent->children.Add(entry);

            // Set last entry
            last = &parent->children.Last();
        }
        else
        {
            // Set parent
            entry.parent = nullptr;

            // Push new entry to root
            treeProfiles.Add(entry);

            // Set last entry
            last = &treeProfiles.Last();
        }
    }
}

void Profiler::Initialize()
{
    InitThread("Main Thread");

    m_profilingEnabled = Environment::GetCommandLineArguments().Contains(STRING_CONST("-profile"));
}

void Profiler::Shutdown()
{
    FinishThread();
    Logger::Log("Profiler disposed");
}

void Profiler::OnUpdate()
{
    CPU_PROFILE_SCOPE(0, __FUNCTION__);

    // Toggle window display when user pressed 'F9' key.
    if (InputManager::IsKeyDown(Key::F9))
        m_showWindow = !m_showWindow;

    // Toggle profiling when window is displayed and user pressed 'P' key.
    if (InputManager::IsKeyDown(Key::P) && m_showWindow)
    {
        ToggleProfiling();
    }

    // Draw window when enabled
    if (m_showWindow)
    {
        DrawWindow();
    }
}

void Profiler::ToggleProfiling()
{
    if (m_profilingEnabled)
        m_stopProfiling = true;
    else
        m_startProfiling = true;
}

void Profiler::ThreadData::BeginFrame()
{
    if (IS_MAIN_THREAD())
    {
        // Stop profiling when `m_stopProfiling` flag is true
        if (GetInstance()->m_stopProfiling)
        {
            GetInstance()->m_profilingEnabled = false;
            GetInstance()->m_stopProfiling = false;
        }

        // Start profiling when `m_startProfiling` flag is true
        if (GetInstance()->m_startProfiling)
        {
            GetInstance()->m_profilingEnabled = true;
            GetInstance()->m_startProfiling = false;
        }
    }

    // Setup new frame
    currentFrame = {};
    currentFrame.startTime_ms = Platform::GetMilliseconds();
}

void Profiler::ThreadData::PushFrame()
{
    currentFrame.endTime_ms = Platform::GetMilliseconds();
    currentFrame.time_ms = float(currentFrame.endTime_ms - currentFrame.startTime_ms);

    // TODO: per-thread profile enable/disable
    // BUG: Per-thread profile enable/disable is needed, because we can get crash, when global profilingEnable state is being changed
}

void Profiler::ThreadData::EndFrame()
{
    ScopeLock(dataLock);

    if (!GetInstance()->m_profilingEnabled && currentFrame.profiles.empty())
        return;

    frames.emplace_back(currentFrame);

    if (frames.size() > NumProfiledFrames)
        frames.erase(frames.begin());
}

uint32_t Profiler::ThreadData::BeginCPUProfile(const char* name)
{
    // Construct profile
    ProfileEntry newProfile;
    newProfile.profileName = name;
    newProfile.startTime_ms = Platform::GetMilliseconds();
    newProfile.depth = static_cast<int>(currentFrame.profileQueue.size());

    // Push new profile
    currentFrame.profileQueue.emplace_back(newProfile);
    return uint32_t(currentFrame.profileQueue.size());
}

void Profiler::ThreadData::EndCPUProfile(const uint32_t profileId)
{
    ASSERT(currentFrame.profileQueue.empty() == false);

    if (profileId != 0)
        ASSERT(currentFrame.profileQueue.size() == profileId);

    const auto currentTime = Platform::GetMilliseconds();

    // Get current profile
    auto currentProfile = currentFrame.profileQueue.back();
    currentProfile.endTime_ms = currentTime;
    currentProfile.profileTime_ms = float(currentTime - currentProfile.startTime_ms);

    // Pop current profile
    currentFrame.profileQueue.pop_back();

    // Push new profile
    dataLock.LockNow();
    currentFrame.profiles.emplace_back(currentProfile);
    dataLock.UnlockNow();
}

void Profiler::ThreadData::Cleanup()
{
    if (currentFrame.profileQueue.empty())
        return;

    frames.clear();
    frames.resize(0);

    currentFrame.profileQueue.clear();
    currentFrame.profileQueue.resize(0);
}

void Profiler::InitThread(const char* threadName)
{
    // Setup thread
    auto thread = new ThreadData();
    thread->threadName = threadName;
    thread->threadId = std::this_thread::get_id();
    thread->opened = strcmp(threadName, "Main Thread") == 0;

    // Add thread
    GetInstance()->m_threads.emplace_back(thread);
    GetInstance()->m_threadMap.insert(std::make_pair(thread->threadId, thread));
}

void Profiler::FinishThread()
{
    for (auto&& thread : GetInstance()->m_threads)
    {
        if (!thread)
            continue;

        delete thread;
        thread = nullptr;
    }

    GetInstance()->m_threadMap.clear();
    GetInstance()->m_threads.clear();
}

void Profiler::BeginFrame()
{
    GetInstance()->GetCurrentThreadData()->BeginFrame();
}

void Profiler::PushFrame()
{
    GetInstance()->GetCurrentThreadData()->PushFrame();
}

void Profiler::EndFrame(const bool autoPush)
{
    if (autoPush)
        GetInstance()->GetCurrentThreadData()->PushFrame();

    GetInstance()->GetCurrentThreadData()->EndFrame();
}
