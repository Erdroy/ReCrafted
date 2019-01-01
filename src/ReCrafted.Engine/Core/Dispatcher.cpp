// ReCrafted (c) 2016-2019 Always Too Late

#include "Dispatcher.h"
#include "Common/Profiler/Profiler.h"

SINGLETON_IMPL(Dispatcher)

void Dispatcher::Dispatch()
{
    Profiler::BeginProfile("Dispatcher::Dispatch");

    DispatchEntry entry;
    while(m_dispatchQueue.try_dequeue(entry))
    {
        entry.callback.Invoke(entry.userdata);
    }

    Profiler::EndProfile();
}

void Dispatcher::OnInit()
{
}

void Dispatcher::Update()
{
    Dispatch();
}

void Dispatcher::OnDispose()
{
}

void Dispatcher::Dispatch(const Action<void, void*> callback, void* userdata)
{
    ASSERT(m_instance);
    m_instance->m_dispatchQueue.enqueue(DispatchEntry{ callback, userdata });
}
