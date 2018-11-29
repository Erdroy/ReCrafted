// ReCrafted (c) 2016-2018 Always Too Late

#include "Profiler.h"

SINGLETON_IMPL(Profiler)

void Profiler::OnInit()
{
    InitThread("MainThread");
}

void Profiler::OnDispose()
{
    FinishThread();
}

void Profiler::Update()
{
    DrawWindow();
}

void Profiler::InitThread(const char* threadName)
{
    // Setup thread
    var thread = new ProfilerThread();
    thread->threadName = threadName;
    thread->threadId = std::this_thread::get_id();

    // Add thread
    m_instance->m_threads.emplace_back(thread);
}

void Profiler::FinishThread()
{
}

void Profiler::NewFrame()
{
}
