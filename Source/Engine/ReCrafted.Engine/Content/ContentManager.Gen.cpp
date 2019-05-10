// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "ContentManager.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static void ContentManager_InternalInitVirtualAsset(Asset* asset) 
    {
        MAIN_THREAD_ONLY();
        ContentManager::InternalInitVirtualAsset(asset);
    }
    
    static void ContentManager_InternalLoadAssetSync(Asset* asset, MonoString* p_assetFile) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(p_assetFile);
        const auto assetFile = MONO_STRING_TO_CSTR(p_assetFile);
        ContentManager::InternalLoadAssetSync(asset, assetFile);
        MONO_FREE(assetFile);
    }
    
    static void ContentManager_InternalLoadAssetAsync(Asset* asset, MonoString* p_assetFile, MonoObject* p_onLoad) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(p_assetFile);
        const auto assetFile = MONO_STRING_TO_CSTR(p_assetFile);
        ASSERT(p_onLoad);
        const auto onLoad = MONO_DELEGATE_TO_ACTION_2(p_onLoad, void, _t0, Asset*, _t1->ToManaged());
        ContentManager::InternalLoadAssetAsync(asset, assetFile, onLoad);
        MONO_FREE(assetFile);
        MONO_DELEGATE_FREE_STUB(onLoad);
    }
};

void ContentManager::InitRuntime() 
{
    API_BIND("ReCrafted.API.Content.ContentManager::InternalInternalInitVirtualAsset", &APIProxy::ContentManager_InternalInitVirtualAsset);
    API_BIND("ReCrafted.API.Content.ContentManager::InternalInternalLoadAssetSync", &APIProxy::ContentManager_InternalLoadAssetSync);
    API_BIND("ReCrafted.API.Content.ContentManager::InternalInternalLoadAssetAsync", &APIProxy::ContentManager_InternalLoadAssetAsync);
}

const char* ContentManager::Fullname() 
{
    return "ReCrafted.API.Content.ContentManager";
}

const char* ContentManager::Name() 
{
    return "ContentManager";
}

const char* ContentManager::Namespace() 
{
    return "ReCrafted.API.Content";
}