// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef TASK_H
#define TASK_H

// includes
#include "ReCrafted.h"
#include "Action.h"
#include "Lock.h"

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
    /**
     * \brief Continue this task with another.
     * \param task The task which will be executed right after current.
     */
    void ContinueWith(Task* task);

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
    static Task* RunTask(Action<void> function);

    /**
    * \brief Creates and runs task with callback.
    * \param function The function which will be run on remote thread.
    * \param function The function which will be called on main thread after the task is completed.
    * \return The created task.
    */
    static Task* RunTask(Action<void> function, Action<void> callback);

    /**
    * \brief Creates task without callback. This task need to be queued.
    * \param function The function which will be run on remote thread.
    * \return The created task.
    */
    static Task* CreateTask(Action<void> function);

    /**
    * \brief Creates task with callback. This task need to be queued.
    * \param function The function which will be run on remote thread.
    * \param function The function which will be called on main thread after the task is completed.
    * \return The created task.
    */
    static Task* CreateTask(Action<void> function, Action<void> callback);

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
        return InternalCreateTask(customTask, userData);
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

    /**
    * \brief Creates custom task with optional userData. This task need to be queued.
    * \param customTask The custom task pointer.
    * \param userData The optional user data pointer.
    * \return The created custom task.
    */
    static Task* CreateTask(ITask* customTask, void* userData = nullptr)
    {
        return InternalCreateTask(customTask, userData);
    }

private:
    static Task* InternalCreateTask(ITask* customTask, void* userData);
};

#endif // TASK_H
