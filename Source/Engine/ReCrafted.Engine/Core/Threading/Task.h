// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Common/Action.h"
#include "Common/Lock.h"

struct ITask
{
public:
    virtual ~ITask() = default;

    virtual void Execute(void* userData) = 0;
    virtual void Finish() {}
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

    Action<void> m_function;
    Action<void> m_callback;

    Lock m_executionLock = {};
    ITask* m_customTask = nullptr;
    void* m_userData = nullptr;
    Task* m_continueWith = nullptr;

public:
    Task() = default;
    ~Task() = default;

private:
    void Run();
    void PrepareForQueue();

public:
    /// <summary>
    ///     Continue this task with another.
    /// </summary>
    /// <param name="task">The task which will be executed right after current.</param>
    void ContinueWith(Task* task);

    /// <summary>
    ///     Queues this task for execution. 
    ///     Warning: make sure that this task is not already queued.
    /// </summary>
    void Queue();

    /// <summary>
    ///     Waits until this task is executing.
    /// </summary>
    void WaitForFinish();

    /// <summary>
    ///     Cancels this task.
    /// </summary>
    /// <remarks>Works only when this task is currently executing.</remarks>
    void Cancel();

    /// <summary>
    ///     Returns true when this task is queued for execution.
    /// </summary>
    bool IsQueued() const
    {
        return m_queued;
    }

public:
    /// <summary>
    ///     Creates and runs task without callback.
    /// </summary>
    /// <param name="function">The function which will be run on remote thread.</param>
    /// <returns>The created task.</returns>
    static Task* RunTask(const Action<void>& function);

    /// <summary>
    ///     Creates and runs task with callback.
    /// </summary>
    /// <param name="function">The function which will be run on remote thread.</param>
    /// <param name="callback">The function which will be called on main thread after the task is completed.</param>
    /// <returns>The created task.</returns>
    static Task* RunTask(const Action<void>& function, const Action<void>& callback);

    /// <summary>
    ///     Creates task without callback. This task need to be queued.
    /// </summary>
    /// <param name="function">The function which will be run on remote thread.</param>
    /// <returns>The created task.</returns>
    static Task* CreateTask(const Action<void>& function);

    /// <summary>
    ///     Creates task with callback. This task need to be queued.
    /// </summary>
    /// <param name="function">The function which will be run on remote thread.</param>
    /// <param name="callback">The function which will be called on main thread after the task is completed.</param>
    /// <returns>The created task.</returns>
    static Task* CreateTask(const Action<void>& function, const Action<void>& callback);

    /// <summary>
    ///     Creates custom task with optional userData.
    /// </summary>
    /// <typeparam name="T">The custom task structure, must inherit from ITask.</typeparam>
    /// <param name="userData">The optional user data pointer</param>
    /// <remarks>The created custom task.</remarks>
    template <typename T>
    static Task* CreateTask(void* userData = nullptr)
    {
        static_assert(std::is_base_of<ITask, T>::value, "T must inherit from ITask");
        const auto customTask = static_cast<ITask*>(new T());
        return InternalCreateTask(customTask, userData);
    }

    /// <summary>
    ///     Creates custom task with optional userData. This task need to be queued.
    /// </summary>
    /// <typeparam name="T">The custom task structure, must inherit from ITask.</typeparam>
    /// <param name="userData">The optional user data pointer</param>
    /// <remarks>The created custom task.</remarks>
    template <typename T>
    static Task* RunTask(void* userData = nullptr)
    {
        auto task = RunTask<T>(userData);
        task.Queue();
        return task;
    }

    /// <summary>
    ///     Creates custom task with optional userData. This task need to be queued.
    /// </summary>
    /// <param name="customTask">The custom task pointer.</param>
    /// <param name="userData">The optional user data pointer.</param>
    /// <returns>The created custom task.</returns>
    static Task* CreateTask(ITask* customTask, void* userData = nullptr)
    {
        return InternalCreateTask(customTask, userData);
    }

private:
    static Task* InternalCreateTask(ITask* customTask, void* userData);
};

