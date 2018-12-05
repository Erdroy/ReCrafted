// ReCrafted (c) 2016-2018 Always Too Late

#include "Profiler.h"
#include "Common/Input/Input.h"
#include "Core/GameInfo.h"

SINGLETON_IMPL(Profiler)

Profiler::ThreadData* Profiler::GetCurrentThreadData()
{
    cvar it = m_threadMap.find(std::this_thread::get_id());
    if (it != m_threadMap.end())
        return it->second;
    return nullptr;
}

void Profiler::CompileProfiles(const std::vector<ProfileEntry>& profiles, Array<ProfileTreeEntry>& treeProfiles)
{
    var lastDepth = 0;
    var lastProfile = std::string();

    for(rvar profile : profiles)
    {
        if(profile.profileName == lastProfile && profile.depth == lastDepth)
        {
            treeProfiles.Last().callNum++;
            continue;
        }

        ProfileTreeEntry entry;
        entry.name = profile.profileName;
        entry.time = profile.profileTime_ms;
        entry.depth = profile.depth;
        entry.callNum = 1;

        treeProfiles.Add(entry);

        if (lastDepth > profile.depth)
            treeProfiles.Last().popTree = true;

        lastProfile = profile.profileName;
        lastDepth = profile.depth;
    }

    treeProfiles.Last().popTree = true;
    treeProfiles.Reverse();
}

void Profiler::OnInit()
{
    InitThread("Main Thread");

    // When -noprofile argument is provided within the CLI, then we are not going to start
    // the profiling by default
    m_profilingEnabled = !GameInfo::ContainsArgument(TEXT_CONST("-noprofiling"));
}

void Profiler::OnDispose()
{
    FinishThread();
}

void Profiler::Update()
{
    // Toggle window display when user pressed 'F9' key.
    if (Input::IsKeyDown(Key_F9))
        m_showWindow = !m_showWindow;

    // Toggle profiling when window is displayed and user pressed 'P' key.
    if(Input::IsKeyDown(Key_P) && m_showWindow)
    {
        ToggleProfiling();
    }

    // Draw window when enabled
    if(m_showWindow)
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
    if(IS_MAIN_THREAD())
    {
        // Stop profiling when `m_stopProfiling` flag is true
        if (m_instance->m_stopProfiling)
        {
            m_instance->m_profilingEnabled = false;
            m_instance->m_stopProfiling = false;
        }

        // Start profiling when `m_startProfiling` flag is true
        if (m_instance->m_startProfiling)
        {
            m_instance->m_profilingEnabled = true;
            m_instance->m_startProfiling = false;
        }
    }

    // Setup new frame
    currentFrame = {};
    currentFrame.startTime_ms = Platform::GetMiliseconds();
}

void Profiler::ThreadData::PushFrame()
{
    currentFrame.endTime_ms = Platform::GetMiliseconds();
    currentFrame.time_ms = float(currentFrame.endTime_ms - currentFrame.startTime_ms);

    // TODO: per-thread profile enable/disable
    // BUG: Per-thread profile enable/disable is needed, because we can get crash, when global profilingEnable state is being changed
}

void Profiler::ThreadData::EndFrame()
{
    ScopeLock(dataLock);

    if (!m_instance->m_profilingEnabled && currentFrame.profiles.empty())
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
    newProfile.startTime_ms = Platform::GetMiliseconds();
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

    cvar currentTime = Platform::GetMiliseconds();

    // Get current profile
    var currentProfile = currentFrame.profileQueue.back();
    currentProfile.endTime_ms = currentTime;
    currentProfile.profileTime_ms = float(currentTime - currentProfile.startTime_ms);

    // Pop current profile
    currentFrame.profileQueue.pop_back();

    // Push new profile
    dataLock.LockNow();
    currentFrame.profiles.emplace_back(currentProfile);
    dataLock.UnlockNow();
}

void Profiler::InitThread(const char* threadName)
{
    // Setup thread
    var thread = new ThreadData();
    thread->threadName = threadName;
    thread->threadId = std::this_thread::get_id();
    thread->opened = strcmp(threadName, "Main Thread") == 0;

    // Add thread
    m_instance->m_threads.emplace_back(thread);
    m_instance->m_threadMap.insert(std::make_pair(thread->threadId, thread));
}

void Profiler::FinishThread()
{
    // TODO: Cleanup and shutdown current ThreadData
}

void Profiler::BeginFrame()
{
    m_instance->GetCurrentThreadData()->BeginFrame();
}

void Profiler::PushFrame()
{
    m_instance->GetCurrentThreadData()->PushFrame();
}

void Profiler::EndFrame(const bool autoPush)
{
    if(autoPush)
        m_instance->GetCurrentThreadData()->PushFrame();

    m_instance->GetCurrentThreadData()->EndFrame();
}
