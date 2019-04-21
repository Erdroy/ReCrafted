// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Common/Logger.h"

template<class TType >
class TestObject
{
    virtual int Destroy(int testParam, const String& param2) const { return 0; }
};

API_CLASS(public, sealed, customName="ClassTest")
class TestClass final : public TestObject<TestClass>
{
    API_CLASS_BODY()

public:
    /**
     * \brief Destroys this actor.
     */
    API_FUNCTION(public, virtual, customName="Destroy")
    int Destroy(int testParam, const String& param2) const override
    {
        Logger::Log(__FUNCTION__);
        delete this;
        return 0;
    }

    API_FUNCTION()
    void TestFunction1()
    {
        
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

public:
    API_PROPERTY()
    int TestProperty1;
};