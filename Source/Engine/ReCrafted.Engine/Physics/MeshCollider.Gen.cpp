// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "MeshCollider.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static void MeshCollider_SetMesh1(MeshCollider* instance, Mesh* mesh) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "MeshCollider");
        instance->SetMesh(mesh);
    }
    
    static void MeshCollider_SetMesh2(MeshCollider* instance, MonoArray* p_vertices, MonoArray* p_indices) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(p_vertices);
        const auto vertices = MONO_ARRAY_TO_ARRAY(p_vertices, Vector3, Vector3, Vector3, _t0);
        ASSERT(p_indices);
        const auto indices = MONO_ARRAY_TO_ARRAY(p_indices, uint32_t, uint32_t, uint32_t, _t0);
        MONO_CHECK_OBJECT(instance, "MeshCollider");
        instance->SetMesh(vertices, indices);
        MONO_FREE_ARRAY(vertices);
        MONO_FREE_ARRAY(indices);
    }
    
    static Object* MeshCollider_CreateObject(bool createManagedInstance)
    {
        _ASSERT_(createManagedInstance, "Class 'MeshCollider' is not marked as generic, and thus cannot get only unmanaged-instance created!");
        return Object::New<MeshCollider>();
    }
};

void MeshCollider::InitRuntime() 
{
    MONO_REGISTER_OBJECT(&APIProxy::MeshCollider_CreateObject);
    
    MONO_REGISTER_OBJECT_TYPE(MeshCollider);
    API_BIND("ReCrafted.API.Physics.MeshCollider::InternalSetMesh", &APIProxy::MeshCollider_SetMesh1);
    API_BIND("ReCrafted.API.Physics.MeshCollider::InternalSetMesh", &APIProxy::MeshCollider_SetMesh2);
}

const char* MeshCollider::Fullname() 
{
    return "ReCrafted.API.Physics.MeshCollider";
}

const char* MeshCollider::Name() 
{
    return "MeshCollider";
}

const char* MeshCollider::Namespace() 
{
    return "ReCrafted.API.Physics";
}