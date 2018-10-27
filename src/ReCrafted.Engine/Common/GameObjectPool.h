// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef GAMEOBJECTPOOL_H
#define GAMEOBJECTPOOL_H

#include "ReCrafted.h"
#include "Common/GameObject.h"
#include "Core/EngineComponent.h"

#include <concurrentqueue.h>

class GameObjectPool : public EngineComponent<GameObjectPool>
{
private:
    friend class EngineMain;

private:
    Array<GameObject*> m_gameObjects;
    moodycamel::ConcurrentQueue<GameObject*> m_releaseGameObjects;
    moodycamel::ConcurrentQueue<GameObject*> m_freeGameObjects;

private:
    void AllocateGameObjects(size_t numGameObjects);
    void CleanupGameObject(GameObject* gameObject);

private:
    void OnInit() override;
    void OnDispose() override;
    void Update() override;
    void LateUpdate() override;
    void OnFrameFinished() override;

    void UpdateGameObjectPool();
    
public:
    /**
     * \brief Ackquires free gameObject.
     * \return The ackquired gameObject.
     */
    static GameObject* AcquireGameObject();

    /**
     * \brief Releases given gameObject and sets it as free for futher use.
     * \param gameObject The gameObject that will be freed.
     */
    static void ReleaseGameObject(GameObject* gameObject);
};

#endif // GAMEOBJECTPOOL_H
