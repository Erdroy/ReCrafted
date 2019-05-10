// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "Asset.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static void Asset_Get_AssetName(Asset* instance, MonoString** p_data) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(instance); 
        const auto _returnValue = instance->AssetName();
        *p_data = MONO_STRING_FROM_CSTR(_returnValue);
    }
    
    static void Asset_Get_AssetFile(Asset* instance, MonoString** p_data) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(instance); 
        const auto _returnValue = instance->AssetFile();
        *p_data = MONO_STRING_FROM_CSTR(_returnValue);
    }
    
    static void Asset_Get_IsVirtual(Asset* instance, bool* data) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(instance); 
        const auto _returnValue = instance->IsVirtual();
        *data = _returnValue;
    }
    
    static void Asset_Get_IsLoaded(Asset* instance, bool* data) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(instance); 
        const auto _returnValue = instance->IsLoaded();
        *data = _returnValue;
    }
};

void Asset::InitRuntime() 
{
    API_BIND("ReCrafted.API.Content.Asset::Get_InternalAssetName", &APIProxy::Asset_Get_AssetName);
    API_BIND("ReCrafted.API.Content.Asset::Get_InternalAssetFile", &APIProxy::Asset_Get_AssetFile);
    API_BIND("ReCrafted.API.Content.Asset::Get_InternalIsVirtual", &APIProxy::Asset_Get_IsVirtual);
    API_BIND("ReCrafted.API.Content.Asset::Get_InternalIsLoaded", &APIProxy::Asset_Get_IsLoaded);
}

const char* Asset::Fullname() 
{
    return "ReCrafted.API.Content.Asset";
}

const char* Asset::Name() 
{
    return "Asset";
}

const char* Asset::Namespace() 
{
    return "ReCrafted.API.Content";
}