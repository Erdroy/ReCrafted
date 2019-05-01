// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "Object.h"
#include "Scripting/Mono.h"

class APIProxy
{
public:

    static void Object_Destroy(Object* objectInstance) 
    {
        MAIN_THREAD_ONLY();
        Object::Destroy(objectInstance);
    }
};

void Object::InitRuntime() 
{
    API_BIND("ReCrafted.API.Object::InternalDestroy", &APIProxy::Object_Destroy);
}