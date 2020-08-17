// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "Model.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static void Model_Get_Materials1(Model* instance, MonoArray** p_data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Model");
        const auto _returnValue = instance->GetMaterials();
        *p_data = MONO_ARRAY_FROM_OBJECT_ARRAY(_returnValue, Material, Material*, MonoObject*, _t0->ToManaged());
    }
    
    static Object* Model_CreateObject(bool createManagedInstance)
    {
        _ASSERT_(createManagedInstance, "Class 'Model' is not marked as generic, and thus cannot get only unmanaged-instance created!");
        return Object::New<Model>();
    }
};

void Model::InitRuntime() 
{
    MONO_REGISTER_OBJECT(&APIProxy::Model_CreateObject);
    
    MONO_REGISTER_OBJECT_TYPE(Model);
    API_BIND("ReCrafted.API.Rendering.Model::Get_InternalMaterials", &APIProxy::Model_Get_Materials1);
}

const char* Model::Fullname() 
{
    return "ReCrafted.API.Rendering.Model";
}

const char* Model::Name() 
{
    return "Model";
}

const char* Model::Namespace() 
{
    return "ReCrafted.API.Rendering";
}