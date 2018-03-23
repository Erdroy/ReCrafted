// ReCrafted (c) 2016-2018 Always Too Late

#include "Task.h"
#include "Platform/Platform.h"
#include "TaskManager.h"

void Task::run()
{
    m_timeStart = Platform::getMiliseconds();

    m_function.Invoke();
    m_completed = true;

    m_timeEnd = Platform::getMiliseconds();
}

Task Task::createTask(Delegate<void> function)
{
    return TaskManager::createTask(function, {});
}

Task Task::createTask(Delegate<void> function, Delegate<bool> callback)
{
    return TaskManager::createTask(function, callback);
}
