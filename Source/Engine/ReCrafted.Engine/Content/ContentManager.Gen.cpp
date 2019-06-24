// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "ContentManager.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static void ContentManager_InternalInitVirtualAsset1(Asset* asset) 
    {
        MAIN_THREAD_ONLY();
        ContentManager::InternalInitVirtualAsset(asset);
    }
    
    static void ContentManager_InternalLoadAssetSync2(Asset* asset, MonoString* p_assetFile) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(p_assetFile);
        const auto assetFile = MONO_STRING_TO_CSTR(p_assetFile);
        ContentManager::InternalLoadAssetSync(asset, assetFile);
        MONO_FREE(assetFile);
    }
    
    static void ContentManager_InternalLoadAssetAsync3(Asset* asset, MonoString* p_assetFile, MonoObject* p_onLoad) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(p_assetFile);
        const auto assetFile = MONO_STRING_TO_CSTR(p_assetFile);
        ASSERT(p_onLoad);
        const auto onLoad = MONO_DELEGATE_TO_ACTION_2(p_onLoad, void, void, void, _t0, Asset, Asset*, MonoObject*, _t1->ToManaged());
        ContentManager::InternalLoadAssetAsync(asset, assetFile, onLoad);
        MONO_FREE(assetFile);
        MONO_FREE_STUB(onLoad);
    }
    
    static MonoObject* ContentManager_InternalFindAsset4(const Guid& guid) 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = ContentManager::InternalFindAsset(guid);
        return _returnValue != nullptr ? _returnValue->ToManaged() : nullptr;
    }
    
    static MonoObject* ContentManager_InternalFindAsset5(MonoString* p_assetFile) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(p_assetFile);
        const auto assetFile = MONO_STRING_TO_CSTR(p_assetFile);
        const auto _returnValue = ContentManager::InternalFindAsset(assetFile);
        MONO_FREE(assetFile);
        return _returnValue != nullptr ? _returnValue->ToManaged() : nullptr;
    }
};

void ContentManager::InitRuntime() 
{
    MONO_REGISTER_OBJECT_TYPE(ContentManager);
    API_BIND("ReCrafted.API.Content.ContentManager::InternalInternalInitVirtualAsset", &APIProxy::ContentManager_InternalInitVirtualAsset1);
    API_BIND("ReCrafted.API.Content.ContentManager::InternalInternalLoadAssetSync", &APIProxy::ContentManager_InternalLoadAssetSync2);
    API_BIND("ReCrafted.API.Content.ContentManager::InternalInternalLoadAssetAsync", &APIProxy::ContentManager_InternalLoadAssetAsync3);
    API_BIND("ReCrafted.API.Content.ContentManager::InternalInternalFindAsset", &APIProxy::ContentManager_InternalFindAsset4);
    API_BIND("ReCrafted.API.Content.ContentManager::InternalInternalFindAsset", &APIProxy::ContentManager_InternalFindAsset5);
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