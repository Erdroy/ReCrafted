// ReCrafted (c) 2016-2018 Always Too Late

#include "Task.h"
#include "Platform/Platform.h"
#include "TaskManager.h"

void Task::Run()
{
    m_timeStart = Platform::GetMiliseconds();

    m_function.Invoke();
    m_completed = true;

    m_timeEnd = Platform::GetMiliseconds();

    // Unlock
    m_executionLock.UnlockNow();
}

void Task::Queue()
{
    // Lock execution
    m_executionLock.LockNow();

    _ASSERT_(m_queued == false, "Task is already queued!");
    TaskManager::QueueTask(*this);
}

void Task::WaitForFinish()
{
    m_executionLock.Enter();
}

void Task::Cancel()
{
    TaskManager::CancelTask(m_id);
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
    return TaskManager::CreateTask(function, {}, false);
}

Task Task::CreateTask(Delegate<void> function, Delegate<bool> callback)
{
    return TaskManager::CreateTask(function, callback, false);
}
