// ReCrafted (c) 2016-2019 Always Too Late

#include "ActorPool.h"
#include "ActorPoolManager.h"

void ActorPoolBase::Register(ActorPoolBase* pool)
{
    ActorPoolManager::Register(this);
}
