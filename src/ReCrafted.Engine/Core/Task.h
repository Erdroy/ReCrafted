// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef TASK_H
#define TASK_H

// includes
#include "ReCrafted.h"
#include "Delegate.h"

struct Task
{
    friend class TaskManager;

private:
    uint m_id = 0u;

    double m_timeQueue = 0.0;
    double m_timeStart = 0.0;
    double m_timeEnd = 0.0;

    bool m_completed = false;

    Delegate<void> m_function;
    Delegate<bool> m_callback;

public:
    void run();

public:
    /**
    * \brief Creates task without calback.
    * \param function The function which will be run on remote thread.
    * \return The created task.
    */
    static Task CreateTask(Delegate<void> function);

    /**
    * \brief Creates task without calback.
    * \param function The function which will be run on remote thread.
    * \param function The function which will be called on main thread after the task is completed.
    * \return The created task.
    */
    static Task CreateTask(Delegate<void> function, Delegate<bool> callback);
};

#endif // TASK_H
