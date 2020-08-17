// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#include "TaskManager.h"
#include "Common/Platform/Platform.h"
#include "Common/Logger.h"
#include "Task.h"
#include "Profiler/Profiler.h"
//#include "Common/Profiler/Profiler.h"

void TaskManager::WorkerFunction()
{
    Platform::SetThreadName("TaskManager Worker");

    Logger::Log("TaskManager thread startup");

    Profiler::InitThread("Task Worker");
    Profiler::BeginFrame();

    Task* task;
    while (m_running)
    {
        ScopeLock(m_globalLock);

        if (!m_taskExecuteQueue.try_dequeue(task))
        {
            Profiler::EndFrame();
            Platform::Sleep(m_sleepTime);
            Profiler::BeginFrame();
            continue;
        }

        Profiler::BeginCPUProfile("Task");

        if(task->m_cancelled)
        {
            // Queue task for release
            m_taskReleaseQueue.enqueue(task);

            // ... and also drop the execution lock
            task->m_executionLock.UnlockNow();
            Profiler::EndFrame();
            Profiler::BeginFrame();
            continue;
        }

        // run task
        task->Run();

        // queue task for release
        m_taskReleaseQueue.enqueue(task);

        Profiler::EndCPUProfile();
    }

    Logger::Log("TaskManager worker thread is exiting");
}

Task* TaskManager::AcquireTask()
{
    Task* task;

    // Try to get task from the pool, 
    // if there is no any free task, create new one
    if (!m_taskPool.try_dequeue(task))
    {
        task = new Task();
        m_tasks.enqueue(task);
    }

    return task;
}

void TaskManager::ReleaseTask(Task* task)
{
    if(!task->m_cancelled)
    {
        // Invoke callback
        if (task->m_callback.IsValid())
            task->m_callback.Invoke();

        if (task->m_customTask)
            task->m_customTask->Finish();
    }

    // Release 'Continue' task
    if (task->m_continueWith)
        ReleaseTask(task->m_continueWith);

    // Clean task
    CleanupTask(task);

    // Return task back to the pool
    m_taskPool.enqueue(task);
}

void TaskManager::Initialize()
{ 
    // Run threads
    const auto maxThreads = Platform::CpuCount() - 1;

    Logger::Log("Starting TaskManager workers.");
    m_running = true;
    for (auto i = 0; i < maxThreads; i++)
    {
        m_workerThreads.Add(new std::thread([&] {
            WorkerFunction();
        }));
    }

    // Initialize tasks
    for (auto i = 0; i < InitialTaskCount; i++)
    {
        const auto task = new Task();
        m_tasks.enqueue(task);
        m_taskPool.enqueue(task);
    }
}

void TaskManager::Shutdown()
{
    m_running = false;

    Logger::Log("Waiting for TaskManager workers to exit...");

    // wait for threads to exit
    for (auto&& thread : m_workerThreads)
    {
        if (thread && thread->joinable())
            thread->join();

        delete thread;
    }

    Task* task;
    while(m_tasks.try_dequeue(task))
    {
        // Cancel all tasks
        CancelTask(task);
        delete task;
    }

    Logger::Log("TaskManager workers exited.");
}

void TaskManager::OnUpdate()
{
    Profiler::BeginProfile("TaskManager::Dispatch");

    Task* task;
    while (m_taskReleaseQueue.try_dequeue(task))
    {
        ReleaseTask(task);
    }

    Profiler::EndProfile();
}

void TaskManager::CleanupTask(Task* task)
{
    delete task->m_customTask;

    // Cleanup 'continue with' task
    if (task->m_continueWith)
        CleanupTask(task->m_continueWith);

    task->m_id = 0u;
    task->m_timeQueue = 0.0f;
    task->m_timeStart = 0.0f;
    task->m_timeEnd = 0.0f;
    task->m_queued = false;
    task->m_completed = false;
    task->m_function = {};
    task->m_callback = {};
    task->m_customTask = nullptr;
    task->m_userData = nullptr;
    task->m_continueWith = nullptr;
}

void TaskManager::CancelTask(Task* task)
{
    if (task->IsQueued())
    {
        // We cannot remove the task from the queue,
        // so instead, we will just set a cancellation flat.
        task->InternalCancel();
        return;
    }


    // We can safely cancel the task here, because we're running on main thread.
    // Also the Finished callback will not be called, because it is also called from the main thread
    // after the task has finished executing (and as we are here, the task has not finished executing).
    task->InternalCancel();

    // Block the main thread until the task finishes executing.
    task->WaitForFinish();
}

void TaskManager::QueueTask(Task* task)
{
    GetInstance()->m_taskExecuteQueue.enqueue(task);
}

Task* TaskManager::CreateTask(const Action<void>& function, const Action<void>& callback)
{
    auto task = GetInstance()->AcquireTask();

    task->m_id = GetInstance()->m_lastId++;
    task->m_function = function;
    task->m_callback = callback;

    return task;
}

Task* TaskManager::CreateTask(ITask* customTask, void* userData)
{
    auto task = GetInstance()->AcquireTask();

    task->m_id = GetInstance()->m_lastId++;
    task->m_customTask = customTask;
    task->m_userData = userData;

    return task;
}
