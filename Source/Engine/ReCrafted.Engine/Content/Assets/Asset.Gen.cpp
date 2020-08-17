// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "Asset.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static void Asset_Get_AssetName1(Asset* instance, MonoString** p_data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Asset");
        const auto _returnValue = instance->AssetName();
        *p_data = MONO_STRING_FROM_CSTR(_returnValue);
    }
    
    static void Asset_Get_AssetFile2(Asset* instance, MonoString** p_data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Asset");
        const auto _returnValue = instance->AssetFile();
        *p_data = MONO_STRING_FROM_CSTR(_returnValue);
    }
    
    static void Asset_Get_IsVirtual3(Asset* instance, bool* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Asset");
        const auto _returnValue = instance->IsVirtual();
        *data = _returnValue;
    }
    
    static void Asset_Get_IsLoaded4(Asset* instance, bool* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Asset");
        const auto _returnValue = instance->IsLoaded();
        *data = _returnValue;
    }
    
    static void Asset_Get_IsLoading5(Asset* instance, bool* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Asset");
        const auto _returnValue = instance->IsLoading();
        *data = _returnValue;
    }
    
    static void Asset_Get_GetGuid6(Asset* instance, Guid* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Asset");
        const auto _returnValue = instance->GetGuid();
        *data = _returnValue;
    }
};

void Asset::InitRuntime() 
{
    MONO_REGISTER_OBJECT_TYPE(Asset);
    API_BIND("ReCrafted.API.Content.Asset::Get_InternalAssetName", &APIProxy::Asset_Get_AssetName1);
    API_BIND("ReCrafted.API.Content.Asset::Get_InternalAssetFile", &APIProxy::Asset_Get_AssetFile2);
    API_BIND("ReCrafted.API.Content.Asset::Get_InternalIsVirtual", &APIProxy::Asset_Get_IsVirtual3);
    API_BIND("ReCrafted.API.Content.Asset::Get_InternalIsLoaded", &APIProxy::Asset_Get_IsLoaded4);
    API_BIND("ReCrafted.API.Content.Asset::Get_InternalIsLoading", &APIProxy::Asset_Get_IsLoading5);
    API_BIND("ReCrafted.API.Content.Asset::Get_InternalGetGuid", &APIProxy::Asset_Get_GetGuid6);
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