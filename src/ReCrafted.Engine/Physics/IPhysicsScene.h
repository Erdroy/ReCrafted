// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef IPHYSICSSCENE_H
#define IPHYSICSSCENE_H

// includes
#include "ReCrafted.h"

struct RayCastHit;
class IPhysicsActor;
class IPhysicsCharacter;

class IPhysicsScene
{
public:
    virtual ~IPhysicsScene() = default;

protected:
    virtual void Update() = 0;
    virtual void Simulate() = 0;
    virtual void Shutdown() = 0;

public:
    virtual void AttachActor(IPhysicsActor* actor) = 0;
    virtual void DetachActor(IPhysicsActor* actor) = 0;

public:
    virtual IPhysicsCharacter* CreateCharacter(float radius, float height, float stepOffset, float slopeLimit, float contactOffset) = 0;
    virtual void ReleaseCharacter(IPhysicsCharacter* character) = 0;

public:
    virtual bool RayCast(const Vector3& position, const Vector3& direction, float maxDistance, RayCastHit* hit, uint32_t collisionLayer = 0) = 0;
};

#endif // IPHYSICSSCENE_H
