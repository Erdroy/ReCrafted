// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "JsonAsset.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
};

void JsonAsset::InitRuntime() 
{
    MONO_REGISTER_OBJECT_TYPE(JsonAsset);
}

const char* JsonAsset::Fullname() 
{
    return "ReCrafted.API.Content.JsonAsset";
}

const char* JsonAsset::Name() 
{
    return "JsonAsset";
}

const char* JsonAsset::Namespace() 
{
    return "ReCrafted.API.Content";
}