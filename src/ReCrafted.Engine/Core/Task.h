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

    float m_timeQueue = 0.0;
    float m_timeStart = 0.0;
    float m_timeEnd = 0.0;

    bool m_queued = false;
    bool m_completed = false;

    Delegate<void> m_function;
    Delegate<bool> m_callback;

    Lock m_executionLock = {};
    ITask* m_customTask = nullptr;
    void* m_userData = nullptr;

public:
    Task() = default;
    ~Task() = default;

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
    * \brief Creates and runs task without callback.
    * \param function The function which will be run on remote thread.
    * \return The created task.
    */
    static Task* RunTask(Delegate<void> function);

    /**
    * \brief Creates and runs task with callback.
    * \param function The function which will be run on remote thread.
    * \param function The function which will be called on main thread after the task is completed.
    * \return The created task.
    */
    static Task* RunTask(Delegate<void> function, Delegate<bool> callback);

    /**
    * \brief Creates task without callback. This task need to be queued.
    * \param function The function which will be run on remote thread.
    * \return The created task.
    */
    static Task* CreateTask(Delegate<void> function);

    /**
    * \brief Creates task with callback. This task need to be queued.
    * \param function The function which will be run on remote thread.
    * \param function The function which will be called on main thread after the task is completed.
    * \return The created task.
    */
    static Task* CreateTask(Delegate<void> function, Delegate<bool> callback);

    /**
     * \brief Creates custom task with optional userData.
     * \tparam T The custom task structure, must inherit from ITask.
     * \param userData The optional user data pointer.
     * \return The created custom task.
     */
    template <typename T>
    static Task* CreateTask(void* userData = nullptr)
    {
        static_assert(std::is_base_of<ITask, T>::value, "T must inherit from ITask");
        cvar customTask = static_cast<ITask*>(new T());
        return CreateTask(customTask, userData);
    }

    /**
    * \brief Creates custom task with optional userData. This task need to be queued.
    * \tparam T The custom task structure, must inherit from ITask.
    * \param userData The optional user data pointer.
    * \return The created custom task.
    */
    template <typename T>
    static Task* RunTask(void* userData = nullptr)
    {
        var task = RunTask<T>(userData);
        task.Queue();
        return task;
    }

private:
    static Task* CreateTask(ITask* customTask, void* userData);
};

#endif // TASK_H
