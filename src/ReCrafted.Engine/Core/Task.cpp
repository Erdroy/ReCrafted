// ReCrafted (c) 2016-2018 Always Too Late

#include "Task.h"
#include "Platform/Platform.h"
#include "TaskManager.h"

void Task::Run()
{
    m_timeStart = static_cast<float>(Platform::GetMiliseconds());

    // Execute custom task, or just a function
    if(m_customTask)
        m_customTask->Execute(m_userData);
    else
        m_function.Invoke();

    // Run 'Continue' task
    if(m_continueWith)
        m_continueWith->Run();

    m_completed = true;
    m_timeEnd = static_cast<float>(Platform::GetMiliseconds());

    // Unlock
    m_executionLock.UnlockNow();
}

void Task::PrepareForQueue()
{
    // Lock execution
    m_executionLock.LockNow();

    // Set queue time
    m_timeQueue = static_cast<float>(Platform::GetMiliseconds());
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
    TaskManager::CancelTask(m_id);
}

Task* Task::RunTask(Delegate<void> function)
{
    cvar task = CreateTask(function, {});
    task->m_queued = true;
    TaskManager::QueueTask(task);
    return task;
}

Task* Task::RunTask(Delegate<void> function, Delegate<bool> callback)
{
    cvar task = CreateTask(function, callback);
    task->m_queued = true;
    TaskManager::QueueTask(task);
    return task;
}

Task* Task::CreateTask(Delegate<void> function)
{
    return TaskManager::CreateTask(function, {});
}

Task* Task::CreateTask(Delegate<void> function, Delegate<bool> callback)
{
    return TaskManager::CreateTask(function, callback);
}

Task* Task::CreateTask(ITask* customTask, void* userData)
{
    ASSERT(customTask != nullptr);

    return TaskManager::CreateTask(customTask, userData);
}
