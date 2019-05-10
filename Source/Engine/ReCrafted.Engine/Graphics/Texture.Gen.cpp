// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "Texture.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static Object* Texture_CreateObject(bool createManagedInstance)
    {
        _ASSERT_(createManagedInstance, "Class 'Texture' is not marked as generic, and thus cannot get only unmanaged-instance created!");
        return Object::New<Texture>();
    }
};

void Texture::InitRuntime() 
{
    MONO_REGISTER_OBJECT(&APIProxy::Texture_CreateObject);
    
}

const char* Texture::Fullname() 
{
    return "ReCrafted.API.Graphics.Texture";
}

const char* Texture::Name() 
{
    return "Texture";
}

const char* Texture::Namespace() 
{
    return "ReCrafted.API.Graphics";
}