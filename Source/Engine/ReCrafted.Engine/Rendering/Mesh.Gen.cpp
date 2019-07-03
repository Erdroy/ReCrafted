// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "Mesh.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static void Mesh_SetVertices1(Mesh* instance, MonoArray* p_vertices) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(p_vertices);
        const auto vertices = MONO_ARRAY_TO_ARRAY(p_vertices, Vector3, Vector3, Vector3, _t0);
        MONO_CHECK_OBJECT(instance, "Mesh");
        instance->SetVertices(vertices);
        MONO_FREE_ARRAY(vertices);
    }
    
    static void Mesh_SetUVs2(Mesh* instance, MonoArray* p_uvs) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(p_uvs);
        const auto uvs = MONO_ARRAY_TO_ARRAY(p_uvs, Vector2, Vector2, Vector2, _t0);
        MONO_CHECK_OBJECT(instance, "Mesh");
        instance->SetUVs(uvs);
        MONO_FREE_ARRAY(uvs);
    }
    
    static void Mesh_SetNormals3(Mesh* instance, MonoArray* p_normals) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(p_normals);
        const auto normals = MONO_ARRAY_TO_ARRAY(p_normals, Vector3, Vector3, Vector3, _t0);
        MONO_CHECK_OBJECT(instance, "Mesh");
        instance->SetNormals(normals);
        MONO_FREE_ARRAY(normals);
    }
    
    static void Mesh_SetColors4(Mesh* instance, MonoArray* p_colors) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(p_colors);
        const auto colors = MONO_ARRAY_TO_ARRAY(p_colors, Vector4, Vector4, Vector4, _t0);
        MONO_CHECK_OBJECT(instance, "Mesh");
        instance->SetColors(colors);
        MONO_FREE_ARRAY(colors);
    }
    
    static void Mesh_SetIndices5(Mesh* instance, MonoArray* p_indices) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(p_indices);
        const auto indices = MONO_ARRAY_TO_ARRAY(p_indices, uint, uint, uint, _t0);
        MONO_CHECK_OBJECT(instance, "Mesh");
        instance->SetIndices(indices);
        MONO_FREE_ARRAY(indices);
    }
    
    static void Mesh_ApplyChanges6(Mesh* instance, bool releaseMeshData, bool autoUpload, bool asyncUpload) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Mesh");
        instance->ApplyChanges(releaseMeshData, autoUpload, asyncUpload);
    }
    
    static void Mesh_Upload7(Mesh* instance, bool asyncUpload) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Mesh");
        instance->Upload(asyncUpload);
    }
    
    static MonoObject* Mesh_CreateMesh8() 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = Mesh::CreateMesh();
        return _returnValue != nullptr ? _returnValue->ToManaged() : nullptr;
    }
    
    static void Mesh_Get_Vertices1(Mesh* instance, MonoArray** p_data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Mesh");
        const auto _returnValue = instance->Vertices();
        *p_data = MONO_ARRAY_FROM_ARRAY(_returnValue, Vector3, Vector3, Vector3, _t0);
    }
    
    static void Mesh_Get_UVs2(Mesh* instance, MonoArray** p_data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Mesh");
        const auto _returnValue = instance->UVs();
        *p_data = MONO_ARRAY_FROM_ARRAY(_returnValue, Vector2, Vector2, Vector2, _t0);
    }
    
    static void Mesh_Get_Normals3(Mesh* instance, MonoArray** p_data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Mesh");
        const auto _returnValue = instance->Normals();
        *p_data = MONO_ARRAY_FROM_ARRAY(_returnValue, Vector3, Vector3, Vector3, _t0);
    }
    
    static void Mesh_Get_Colors4(Mesh* instance, MonoArray** p_data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Mesh");
        const auto _returnValue = instance->Colors();
        *p_data = MONO_ARRAY_FROM_ARRAY(_returnValue, Vector4, Vector4, Vector4, _t0);
    }
    
    static void Mesh_Get_Indices5(Mesh* instance, MonoArray** p_data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Mesh");
        const auto _returnValue = instance->Indices();
        *p_data = MONO_ARRAY_FROM_ARRAY(_returnValue, uint, uint, uint, _t0);
    }
    
    static void Mesh_Get_VertexCount6(Mesh* instance, int* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Mesh");
        const auto _returnValue = instance->VertexCount();
        *data = _returnValue;
    }
    
    static void Mesh_Get_IndexCount7(Mesh* instance, int* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Mesh");
        const auto _returnValue = instance->IndexCount();
        *data = _returnValue;
    }
    
    static void Mesh_Get_IsUploaded8(Mesh* instance, bool* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Mesh");
        const auto _returnValue = instance->IsUploaded();
        *data = _returnValue;
    }
    
    static void Mesh_Get_CanUpload9(Mesh* instance, bool* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Mesh");
        const auto _returnValue = instance->CanUpload();
        *data = _returnValue;
    }
    
    static Object* Mesh_CreateObject(bool createManagedInstance)
    {
        _ASSERT_(createManagedInstance, "Class 'Mesh' is not marked as generic, and thus cannot get only unmanaged-instance created!");
        return Object::New<Mesh>();
    }
};

void Mesh::InitRuntime() 
{
    MONO_REGISTER_OBJECT(&APIProxy::Mesh_CreateObject);
    
    MONO_REGISTER_OBJECT_TYPE(Mesh);
    API_BIND("ReCrafted.API.Rendering.Mesh::InternalSetVertices", &APIProxy::Mesh_SetVertices1);
    API_BIND("ReCrafted.API.Rendering.Mesh::InternalSetUVs", &APIProxy::Mesh_SetUVs2);
    API_BIND("ReCrafted.API.Rendering.Mesh::InternalSetNormals", &APIProxy::Mesh_SetNormals3);
    API_BIND("ReCrafted.API.Rendering.Mesh::InternalSetColors", &APIProxy::Mesh_SetColors4);
    API_BIND("ReCrafted.API.Rendering.Mesh::InternalSetIndices", &APIProxy::Mesh_SetIndices5);
    API_BIND("ReCrafted.API.Rendering.Mesh::InternalApplyChanges", &APIProxy::Mesh_ApplyChanges6);
    API_BIND("ReCrafted.API.Rendering.Mesh::InternalUpload", &APIProxy::Mesh_Upload7);
    API_BIND("ReCrafted.API.Rendering.Mesh::InternalCreateMesh", &APIProxy::Mesh_CreateMesh8);
    API_BIND("ReCrafted.API.Rendering.Mesh::Get_InternalVertices", &APIProxy::Mesh_Get_Vertices1);
    API_BIND("ReCrafted.API.Rendering.Mesh::Get_InternalUVs", &APIProxy::Mesh_Get_UVs2);
    API_BIND("ReCrafted.API.Rendering.Mesh::Get_InternalNormals", &APIProxy::Mesh_Get_Normals3);
    API_BIND("ReCrafted.API.Rendering.Mesh::Get_InternalColors", &APIProxy::Mesh_Get_Colors4);
    API_BIND("ReCrafted.API.Rendering.Mesh::Get_InternalIndices", &APIProxy::Mesh_Get_Indices5);
    API_BIND("ReCrafted.API.Rendering.Mesh::Get_InternalVertexCount", &APIProxy::Mesh_Get_VertexCount6);
    API_BIND("ReCrafted.API.Rendering.Mesh::Get_InternalIndexCount", &APIProxy::Mesh_Get_IndexCount7);
    API_BIND("ReCrafted.API.Rendering.Mesh::Get_InternalIsUploaded", &APIProxy::Mesh_Get_IsUploaded8);
    API_BIND("ReCrafted.API.Rendering.Mesh::Get_InternalCanUpload", &APIProxy::Mesh_Get_CanUpload9);
}

const char* Mesh::Fullname() 
{
    return "ReCrafted.API.Rendering.Mesh";
}

const char* Mesh::Name() 
{
    return "Mesh";
}

const char* Mesh::Namespace() 
{
    return "ReCrafted.API.Rendering";
}