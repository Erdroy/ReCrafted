// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "WebUIManager.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static MonoObject* WebUIManager_CreateView1(int width, int height) 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = WebUIManager::CreateView(width, height);
        return _returnValue != nullptr ? _returnValue->ToManaged() : nullptr;
    }
};

void WebUIManager::InitRuntime() 
{
    MONO_REGISTER_OBJECT_TYPE(WebUIManager);
    API_BIND("ReCrafted.API.WebUI.WebUIManager::InternalCreateView", &APIProxy::WebUIManager_CreateView1);
}

const char* WebUIManager::Fullname() 
{
    return "ReCrafted.API.WebUI.WebUIManager";
}

const char* WebUIManager::Name() 
{
    return "WebUIManager";
}

const char* WebUIManager::Namespace() 
{
    return "ReCrafted.API.WebUI";
}