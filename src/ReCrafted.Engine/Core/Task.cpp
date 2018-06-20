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

    m_completed = true;
    m_timeEnd = static_cast<float>(Platform::GetMiliseconds());

    // Unlock
    m_executionLock.UnlockNow();

    delete m_customTask;
}

void Task::Queue()
{
    // Lock execution
    m_executionLock.LockNow();

    // Set queue time
    m_timeQueue = static_cast<float>(Platform::GetMiliseconds());

    _ASSERT_(m_queued == false, "Task is already queued!");
    TaskManager::QueueTask(*this);
}

void Task::WaitForFinish()
{
    m_executionLock.Enter();
}

void Task::Cancel()
{
    if(TaskManager::CancelTask(m_id))
    {
        // Delete custom task when everything is done
        delete m_customTask;
    }
}

Task Task::RunTask(Delegate<void> function)
{
    var task = CreateTask(function, {});
    task.m_queued = true;
    TaskManager::QueueTask(task);
    return task;
}

Task Task::RunTask(Delegate<void> function, Delegate<bool> callback)
{
    var task = CreateTask(function, callback);
    task.m_queued = true;
    TaskManager::QueueTask(task);
    return task;
}

Task Task::CreateTask(Delegate<void> function)
{
    return TaskManager::CreateTask(function, {});
}

Task Task::CreateTask(Delegate<void> function, Delegate<bool> callback)
{
    return TaskManager::CreateTask(function, callback);
}

Task Task::CreateTask(ITask* customTask, void* userData)
{
    return TaskManager::CreateTask(customTask, userData);
}
