// ReCrafted (c) 2016-2018 Always Too Late

#include "Task.h"
#include "Platform/Platform.h"
#include "TaskManager.h"

void Task::run()
{
    m_timeStart = Platform::GetMiliseconds();

    m_function.Invoke();
    m_completed = true;

    m_timeEnd = Platform::GetMiliseconds();
}

Task Task::CreateTask(Delegate<void> function)
{
    return TaskManager::CreateTask(function, {});
}

Task Task::CreateTask(Delegate<void> function, Delegate<bool> callback)
{
    return TaskManager::CreateTask(function, callback);
}
