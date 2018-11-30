// ReCrafted (c) 2016-2018 Always Too Late

#include "Profiler.h"
#include "Common/Input/Input.h"
#include "Core/GameInfo.h"

SINGLETON_IMPL(Profiler)

void Profiler::OnInit()
{
    InitThread("MainThread");

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
        m_profilingEnabled = !m_profilingEnabled;

    // Draw window when enabled
    if(m_showWindow)
    {
        DrawWindow();
    }
}


void Profiler::ThreadData::BeginFrame()
{
    currentFrame = {};
    currentFrame.startTime_ms = Platform::GetMiliseconds();
}

void Profiler::ThreadData::EndFrame()
{
    currentFrame.endTime_ms = Platform::GetMiliseconds();
    currentFrame.time_ms = float(currentFrame.endTime_ms - currentFrame.startTime_ms);
    frames.emplace_back(std::move(currentFrame));

    if (frames.size() > 120 * ProfileSeconds)
        frames.erase(frames.begin());
}

uint32_t Profiler::ThreadData::BeginCPUProfile(const char* name)
{
    // Construct profile
    ProfileEntry newProfile;
    newProfile.profileName = name;
    newProfile.startTime_ms = Platform::GetMiliseconds();

    // Push new profile
    currentFrame.profileQueue.push(std::move(newProfile));
    return currentFrame.profileQueue.size();
}

void Profiler::ThreadData::EndCPUProfile(const uint32_t profileId)
{
    if (profileId != 0)
        ASSERT(currentFrame.profileQueue.size() == profileId);

    cvar currentTime = Platform::GetMiliseconds();

    // Get current profile
    var currentProfile = std::move(currentFrame.profileQueue.back());
    currentProfile.endTime_ms = currentTime;
    currentProfile.profileTime_ms = float(currentTime - currentProfile.startTime_ms);

    // Pop current profile
    currentFrame.profileQueue.pop();

    // Push new profile
    currentFrame.profiles.emplace_back(std::move(currentProfile));
}

void Profiler::InitThread(const char* threadName)
{
    // Setup thread
    var thread = new ThreadData();
    thread->threadName = threadName;
    thread->threadId = std::this_thread::get_id();

    // Add thread
    m_instance->m_threads.emplace_back(thread);
    m_instance->m_threadMap.insert(std::make_pair(thread->threadId, thread));
}

void Profiler::FinishThread()
{
    // TODO: Cleanup and shutdown current ThreadData
}

void Profiler::NewFrame()
{
    m_instance->GetCurrentThreadData()->EndFrame();
    m_instance->GetCurrentThreadData()->BeginFrame();
    // TODO: End current frame in current thread
    // TODO: Start new frame
}
