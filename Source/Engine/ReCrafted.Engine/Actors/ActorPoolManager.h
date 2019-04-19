// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Common/List.h"
#include "Actors/ActorPool.h"
#include "SubSystems/SubSystem.h"

class ActorPoolManager : public SubSystem<ActorPoolManager>
{
private:
    List<ActorPoolBase*> m_actorPools;

private:
    void RefreshActorPools();

public:
    void Initialize() override;
    void Shutdown() override;
    void OnFrameDone() override;

public:
    static void Register(ActorPoolBase* pool);

    /**
     * \brief Returns ActorPool for given TActor type.
     * \note This is a template singleton constructor.
     * This allows to instantiate at most only one ActorPool per type 
     * and it is deleted in ActorPoolManager::Shutdown.
     */
    template <typename TActor>
    static ActorPool<TActor>* GetPool()
    {
        static ActorPool<TActor>* pool;

        if (pool == nullptr) 
        {
            pool = new ActorPool<TActor>();
            Register(pool);
        }

        return pool;
    }
};
