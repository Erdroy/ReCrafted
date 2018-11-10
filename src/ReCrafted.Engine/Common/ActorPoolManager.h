// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef ACTORPOOLMANAGER_H
#define ACTORPOOLMANAGER_H

#include "ReCrafted.h"
#include "Core/EngineComponent.h"
#include "ActorPool.h"

class ActorPoolManager : public EngineComponent<ActorPoolManager>
{
private:
    Array<ActorPoolBase*> m_actorPools;

private:
    void RefreshActorPools();

public:
    void OnInit() override;
    void OnDispose() override;
    void OnFrameFinished() override;

public:
    static void Register(ActorPoolBase* pool);

    template <typename TActor>
    static ActorPool<TActor>* GetPool()
    {
        static ActorPool<TActor> pool;
        return &pool;
    }
};

#endif // ACTORPOOLMANAGER_H
