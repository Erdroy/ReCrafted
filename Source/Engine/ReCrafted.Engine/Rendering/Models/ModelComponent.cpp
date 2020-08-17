// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#include "ModelComponent.h"
#include "ModelRenderingSystem.h"

void ModelComponent::BeginModify()
{
    ModelRenderingSystem::GetMainLock()->LockNow();
}

void ModelComponent::EndModify()
{
    ModelRenderingSystem::GetMainLock()->UnlockNow();
}
