// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Common/Logger.h"

template<class TType >
class TestObject
{
    virtual void Destroy() const {}
};

API_CLASS(public, sealed, customName="ClassTest")
class TestClass : public TestObject<TestClass>
{
    API_CLASS_BODY()

public:
    /**
     * \brief Destroys this actor.
     */
    API_FUNCTION(public, virtual, override, customName="Destroy")
    void Destroy() const override
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