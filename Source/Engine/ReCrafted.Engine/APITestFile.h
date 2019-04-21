// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Common/Logger.h"

API_CLASS(public)
class TestClass
{
    API_CLASS_BODY();

public:
    /**
     * \brief Destroys this actor. Works the same as `Object::Destroy(actor)`.
     */
    API_FUNCTION()
    void Destroy() const
    {
        Logger::Log(__FUNCTION__);
        delete this;
    }

public:
    /**
     * \brief Creates actor of type EmptyActor.
     */
    API_FUNCTION()
    static TestClass* Create()
    {
        Logger::Log(__FUNCTION__);
        return new TestClass;
    }
};