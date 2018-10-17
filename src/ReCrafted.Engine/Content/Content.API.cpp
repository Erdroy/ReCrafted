// ReCrafted (c) 2016-2018 Always Too Late

#include "ContentManager.h"
#include "Scripting/Mono.h"
#include "Common/ReCraftedAPI.h"

namespace Internal
{
    Asset* LoadAsset(Asset* asset, MonoString* string)
    {
        // convert monostring to ansi string
        MONO_ANSI_ERR();
        cvar assetFile = MONO_ANSI(string);

        // Load asset
        ContentManager::InternalLoadAssetSync(asset, assetFile);

        // free ansi string
        MONO_ANSI_FREE(assetFile);

        return nullptr;
    }

    Asset* LoadAssetAsync(Asset* asset, MonoString* string, MonoDelegate* delegate)
    {
        // convert monostring to ansi string
        MONO_ANSI_ERR();
        cvar assetFile = MONO_ANSI(string);

        // TODO: Create action from delegate (how?)
        // TODO: Load asset async

        // free ansi string
        MONO_ANSI_FREE(assetFile);

        return nullptr;
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
                API_PARAM("Action<Asset>", "onLoad");

                API_RETURN("Asset");
            }
            API_METHOD_END();
        }
        API_CLASS_END();
    }
    API_FILE_END();
}
