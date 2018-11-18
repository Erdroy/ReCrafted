// ReCrafted (c) 2016-2018 Always Too Late

#include "ActorPool.h"
#include "ActorPoolManager.h"

void ActorPoolBase::Register(ActorPoolBase* pool)
{
    ActorPoolManager::Register(this);
}
