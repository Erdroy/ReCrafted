// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Common/Logger.h"

template<class TType >
class TestObject
{
    virtual int Destroy(int testParam) const { return 0; }
};

API_CLASS(public, sealed)
class TestClass final : public TestObject<TestClass>
{
    API_CLASS_BODY()

public:
    /**
     * \brief Destroys this actor.
     */
    API_FUNCTION(public, virtual)
    int Destroy(int testParam) const override
    {
        Logger::Log(__FUNCTION__);
        delete this;
        return 0;
    }

    API_FUNCTION()
    void TestFunction()
    {

    }

    API_FUNCTION()
    void TestFunction1(const Vector3& vector)
    {
        
    }

    API_FUNCTION()
    void TestFunction2(TestClass* object)
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