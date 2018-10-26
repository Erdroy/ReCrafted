// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef ACTORPOOL_H
#define ACTORPOOL_H

#include "ReCrafted.h"
#include "Common/Actor.h"
#include "Core/EngineComponent.h"

#include <concurrentqueue.h>

class ActorPool : public EngineComponent<ActorPool>
{
private:
    friend class EngineMain;

private:
    Array<Actor*> m_actors;
    moodycamel::ConcurrentQueue<Actor*> m_releaseActors;
    moodycamel::ConcurrentQueue<Actor*> m_freeActors;

private:
    void AllocateActors(size_t numActors);
    void CleanupActor(Actor* actor);

private:
    void OnInit() override;
    void OnDispose() override;
    void Update() override;
    void LateUpdate() override;
    void OnFrameFinished() override;

    void UpdateActorPool();
    
public:
    /**
     * \brief Ackquires free actor.
     * \return The ackquired actor.
     */
    static Actor* AcquireActor();

    /**
     * \brief Releases given actor and sets it as free for futher use.
     * \param actor The actor that will be freed.
     */
    static void ReleaseActor(Actor* actor);
};

#endif // ACTORPOOL_H
