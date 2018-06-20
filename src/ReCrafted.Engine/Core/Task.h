// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef TASK_H
#define TASK_H

// includes
#include "ReCrafted.h"
#include "Delegate.h"
#include "Lock.h"

struct ITask
{
public:
    virtual ~ITask() = default;

    virtual void Execute(void* userData) = 0;
};

struct Task
{
    friend class TaskManager;

private:
    uint m_id = 0u;

    double m_timeQueue = 0.0;
    double m_timeStart = 0.0;
    double m_timeEnd = 0.0;

    bool m_queued = false;
    bool m_completed = false;

    Delegate<void> m_function;
    Delegate<bool> m_callback;

    Lock m_executionLock = {};

private:
    void Run();

public:
    /**
     * \brief Queues this task for execution. 
     * Warning: make sure that this task is not already queued.
     */
    void Queue();

    /**
     * \brief Waits until this task is executing.
     */
    void WaitForFinish();

    /**
     * \brief Cancels this task.
     * Info: Works only when this task is currently executing.
     */
    void Cancel();

    /**
     * \brief Returns true when this task is queued for execution.
     */
    bool IsQueued() const
    {
        return m_queued;
    }

public:
    /**
    * \brief Creates and runs task without calback.
    * \param function The function which will be run on remote thread.
    * \return The created task.
    */
    static Task RunTask(Delegate<void> function);

    /**
    * \brief Creates and runs task without calback.
    * \param function The function which will be run on remote thread.
    * \param function The function which will be called on main thread after the task is completed.
    * \return The created task.
    */
    static Task RunTask(Delegate<void> function, Delegate<bool> callback);

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
