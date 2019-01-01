// ReCrafted (c) 2016-2019 Always Too Late

#include "ContentManager.h"
#include "Scripting/Mono.h"
#include "Common/ReCraftedAPI.h"

namespace Internal
{
    MonoObject* CreateVirtual(Asset* asset)
    {
        ContentManager::InternalInitVirtualAsset(asset);
        return asset->GetManagedPtr();
    }

    MonoObject* LoadAsset(Asset* asset, MonoString* string)
    {
        // convert monostring to ansi string
        MONO_ANSI_ERR();
        cvar assetFile = MONO_ANSI(string);

        // Load asset
        ContentManager::InternalLoadAssetSync(asset, assetFile);

        // free ansi string
        MONO_ANSI_FREE(assetFile);

        return asset->GetManagedPtr();
    }

    void LoadAssetAsync(Asset* asset, MonoString* string, MonoObject* onLoadDelegate)
    {
        // convert monostring to ansi string
        MONO_ANSI_ERR();
        cvar assetFile = MONO_ANSI(string);

        // Create asset async loading callback proxy
        cvar callbackProxy = [onLoadDelegate](Asset* asset)
        {
            var param = asset->GetManagedPtr();
            mono_runtime_delegate_invoke(onLoadDelegate, reinterpret_cast<void**>(&param), nullptr);
        };

        // Load asset
        ContentManager::InternalLoadAssetAsync(asset, assetFile, Action<void, Asset*>(callbackProxy));

        // free ansi string
        MONO_ANSI_FREE(assetFile);
    }
}

void ContentManager::InitRuntime()
{
    API_FILE("Content/ContentManagerInternals.Gen.cs");
    {
        API_COMMENT("ContentManagerInternals class.");
        API_CLASS(INTERNAL, STATIC, "ReCrafted.API.Content", "ContentManagerInternals");
        {
            API_METHOD(PUBLIC, STATIC, "UnloadAsset", EXTERN);
            {
                API_BIND("ReCrafted.API.Content.ContentManagerInternals::UnloadAsset", &ContentManager::UnloadAsset);
                API_PARAM("IntPtr", "assetNativePtr");
            }
            API_METHOD_END();

            API_METHOD(PUBLIC, STATIC, "CreateVirtual", EXTERN);
            {
                API_BIND("ReCrafted.API.Content.ContentManagerInternals::CreateVirtual", &Internal::CreateVirtual);
                API_PARAM("IntPtr", "assetNativePtr");

                API_RETURN("Asset");
            }
            API_METHOD_END();

            API_METHOD(PUBLIC, STATIC, "LoadAsset", EXTERN);
            {
                API_BIND("ReCrafted.API.Content.ContentManagerInternals::LoadAsset", &Internal::LoadAsset);
                API_PARAM("IntPtr", "assetNativePtr");
                API_PARAM("string", "assetFile");

                API_RETURN("Asset");
            }
            API_METHOD_END();

            API_METHOD(PUBLIC, STATIC, "LoadAssetAsync", EXTERN);
            {
                API_BIND("ReCrafted.API.Content.ContentManagerInternals::LoadAssetAsync", &Internal::LoadAssetAsync);
                API_PARAM("IntPtr", "assetNativePtr");
                API_PARAM("string", "assetFile");
                API_PARAM("AssetLoadCallback", "onLoad");
            }
            API_METHOD_END();
        }
        API_CLASS_END();
    }
    API_FILE_END();
}
