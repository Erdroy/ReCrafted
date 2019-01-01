// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef SPACEOBJECTMANAGER_H
#define SPACEOBJECTMANAGER_H

// includes
#include "ReCrafted.h"
#include "Core/Action.h"
#include "Core/Containers/Array.h"

#include <thread>
#include <atomic>

class SpaceObjectOctreeNode;
class SpaceObjectChunk;

struct ProcessMode
{
    enum _enum
    {
        Populate,
        Depopulate,
        Rebuild
    };
};

class SpaceObjectManager : public Singleton<SpaceObjectManager>
{
private:
    Array<std::thread*> m_workerThreads = {};
    std::atomic<bool> m_running = false;

public:
    virtual ~SpaceObjectManager()
    {
    }

private:
    void OnDispose() override;

public:
    void WorkerFunction();

public:
    /**
    * \brief Initializes the SpaceObjectManager.
    */
    void Init();

    /**
    * \brief Updates the SpaceObjectManager.
    * This will invoke all node generation callbacks.
    */
    void Update();

public:
    /**
     * \brief Enqueues node for generation.
     */
    static void Enqueue(SpaceObjectOctreeNode* node, ProcessMode::_enum mode, Action<void> callback);
};

#endif // SPACEOBJECTMANAGER_H
