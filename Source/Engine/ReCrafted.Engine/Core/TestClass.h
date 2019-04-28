// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Common/Logger.h"

class Object {};

API_CLASS(public, sealed)
class TestClass final : public Object
{
    API_CLASS_BODY()

public:
    /**
     * \brief Destroys this actor.
     */
    API_FUNCTION(public, virtual)
    int Destroy(int testParam) const
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
    void TestFunction2(TestClass* obj)
    {

    }

    API_FUNCTION()
    void TestFunction3(Vector3& vector, const Vector3& vector1)
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