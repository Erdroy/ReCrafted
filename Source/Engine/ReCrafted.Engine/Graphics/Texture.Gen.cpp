// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "Texture.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static void Texture_InitializeFromFile1(Texture* instance, MonoString* p_fileName) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(p_fileName);
        const auto fileName = MONO_STRING_TO_CSTR(p_fileName);
        MONO_CHECK_OBJECT(instance, "Texture");
        instance->InitializeFromFile(fileName);
        MONO_FREE(fileName);
    }
    
    static uint Texture_GetPixel2(Texture* instance, int x, int y) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Texture");
        const auto _returnValue = instance->GetPixel(x, y);
        return _returnValue;
    }
    
    static void Texture_SetPixel3(Texture* instance, int x, int y, uint pixel) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Texture");
        instance->SetPixel(x, y, pixel);
    }
    
    static void Texture_SetPixel4(Texture* instance, int x, int y, byte r, byte g, byte b, byte a) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Texture");
        instance->SetPixel(x, y, r, g, b, a);
    }
    
    static void Texture_Resize5(Texture* instance, uint16_t width, uint16_t height) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Texture");
        instance->Resize(width, height);
    }
    
    static void Texture_Get_Width1(Texture* instance, uint* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Texture");
        const auto _returnValue = instance->GetWidth();
        *data = _returnValue;
    }
    
    static void Texture_Get_Height2(Texture* instance, uint* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Texture");
        const auto _returnValue = instance->GetHeight();
        *data = _returnValue;
    }
    
    static void Texture_Get_Is3D3(Texture* instance, bool* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Texture");
        const auto _returnValue = instance->Is3D();
        *data = _returnValue;
    }
    
    static Object* Texture_CreateObject(bool createManagedInstance)
    {
        _ASSERT_(createManagedInstance, "Class 'Texture' is not marked as generic, and thus cannot get only unmanaged-instance created!");
        return Object::New<Texture>();
    }
};

void Texture::InitRuntime() 
{
    MONO_REGISTER_OBJECT(&APIProxy::Texture_CreateObject);
    
    API_BIND("ReCrafted.API.Graphics.Texture::InternalInitializeFromFile", &APIProxy::Texture_InitializeFromFile1);
    API_BIND("ReCrafted.API.Graphics.Texture::InternalGetPixel", &APIProxy::Texture_GetPixel2);
    API_BIND("ReCrafted.API.Graphics.Texture::InternalSetPixel", &APIProxy::Texture_SetPixel3);
    API_BIND("ReCrafted.API.Graphics.Texture::InternalSetPixel", &APIProxy::Texture_SetPixel4);
    API_BIND("ReCrafted.API.Graphics.Texture::InternalResize", &APIProxy::Texture_Resize5);
    API_BIND("ReCrafted.API.Graphics.Texture::Get_InternalWidth", &APIProxy::Texture_Get_Width1);
    API_BIND("ReCrafted.API.Graphics.Texture::Get_InternalHeight", &APIProxy::Texture_Get_Height2);
    API_BIND("ReCrafted.API.Graphics.Texture::Get_InternalIs3D", &APIProxy::Texture_Get_Is3D3);
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