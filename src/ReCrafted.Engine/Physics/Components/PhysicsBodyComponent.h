// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef PHYSICSBODYCOMPONENT_H
#define PHYSICSBODYCOMPONENT_H

#include "Common/Entities/Component.h"

class IPhysicsActor;
class IPhysicsScene;

struct PhysicsBodyComponent : Component
{
    enum Type : uint8_t
    {
        Dynamic,
        Static
    };

public:
    PhysicsBodyComponent() = default;

    explicit PhysicsBodyComponent(IPhysicsScene* scene, const Type type) :
        physicsScene(scene), type(type)
    {
        
    }

public:
    IPhysicsScene* physicsScene = nullptr;
    IPhysicsActor* physicsActor = nullptr;

public:
    Type type = Dynamic;
};

#endif // PHYSICSBODYCOMPONENT_H
