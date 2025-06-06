// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#include "Task.h"
#include "Common/Platform/Platform.h"
#include "TaskManager.h"

void Task::Run()
{
    m_timeStart = static_cast<float>(Platform::GetMilliseconds());

    // Execute custom task, or just a function
    if(m_customTask)
        m_customTask->Execute(m_userData);
    else
        m_function.Invoke();

    // Run 'Continue' task
    if(m_continueWith)
        m_continueWith->Run();

    m_completed = true;
    m_timeEnd = static_cast<float>(Platform::GetMilliseconds());

    // Unlock
    m_executionLock.UnlockNow();
}

void Task::PrepareForQueue()
{
    // Lock execution
    m_executionLock.LockNow();

    // Set queue time
    m_timeQueue = static_cast<float>(Platform::GetMilliseconds());
}

void Task::InternalCancel()
{
    m_cancelled = true;

    if(m_customTask)
    {
        m_customTask->OnCancel();
    }

    if (m_continueWith)
        m_continueWith->Cancel();
}

void Task::ContinueWith(Task* task)
{
    ASSERT(m_continueWith == nullptr);
    ASSERT(task != nullptr);
    ASSERT(task->m_queued == false);

    m_continueWith = task;
    m_continueWith->PrepareForQueue();
}

void Task::Queue()
{
    _ASSERT_(m_queued == false, "Task is already queued!");

    PrepareForQueue();

    TaskManager::QueueTask(this);
}

void Task::WaitForFinish()
{
    m_executionLock.Enter();
}

void Task::Cancel()
{
    TaskManager::CancelTask(this);
}

Task* Task::RunTask(const Action<void>& function)
{
    const auto task = CreateTask(function, {});
    task->m_queued = true;
    TaskManager::QueueTask(task);
    return task;
}

Task* Task::RunTask(const Action<void>& function, const Action<void>& callback)
{
    const auto task = CreateTask(function, callback);
    task->m_queued = true;
    TaskManager::QueueTask(task);
    return task;
}

Task* Task::CreateTask(const Action<void>& function)
{
    return TaskManager::CreateTask(function, {});
}

Task* Task::CreateTask(const Action<void>& function, const Action<void>& callback)
{
    return TaskManager::CreateTask(function, callback);
}

Task* Task::InternalCreateTask(ITask* customTask, void* userData)
{
    ASSERT(customTask != nullptr);

    return TaskManager::CreateTask(customTask, userData);
}
