// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "Camera.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static void Camera_SetAsCurrent1(Camera* instance) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Camera");
        instance->SetAsCurrent();
    }
    
    static void Camera_SetLookAt2(Camera* instance, const Vector3& lookAt) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Camera");
        instance->SetLookAt(lookAt);
    }
    
    static MonoObject* Camera_Create3() 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = Camera::Create();
        return _returnValue != nullptr ? _returnValue->ToManaged() : nullptr;
    }
    
    static void Camera_Get_BoundingFrustum1(Camera* instance, BoundingFrustum* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Camera");
        const auto _returnValue = instance->GetBoundingFrustum();
        *data = _returnValue;
    }
    
    static void Camera_Get_Projection2(Camera* instance, Matrix* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Camera");
        const auto _returnValue = instance->GetProjection();
        *data = _returnValue;
    }
    
    static void Camera_Get_View3(Camera* instance, Matrix* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Camera");
        const auto _returnValue = instance->GetView();
        *data = _returnValue;
    }
    
    static void Camera_Get_ViewProjection4(Camera* instance, Matrix* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Camera");
        const auto _returnValue = instance->GetViewProjection();
        *data = _returnValue;
    }
    
    static void Camera_Get_Position5(Camera* instance, Vector3* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Camera");
        const auto _returnValue = instance->Position();
        *data = _returnValue;
    }

    static void Camera_Set_Position5(Camera* instance, Vector3* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Camera");
        instance->Position(*data);
    }
    
    static void Camera_Get_Rotation6(Camera* instance, Quaternion* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Camera");
        const auto _returnValue = instance->Rotation();
        *data = _returnValue;
    }

    static void Camera_Set_Rotation6(Camera* instance, Quaternion* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Camera");
        instance->Rotation(*data);
    }
    
    static void Camera_Get_Fov7(Camera* instance, float* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Camera");
        const auto _returnValue = instance->Fov();
        *data = _returnValue;
    }

    static void Camera_Set_Fov7(Camera* instance, float* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Camera");
        instance->Fov(*data);
    }
    
    static void Camera_Get_NearPlane8(Camera* instance, float* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Camera");
        const auto _returnValue = instance->NearPlane();
        *data = _returnValue;
    }

    static void Camera_Set_NearPlane8(Camera* instance, float* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Camera");
        instance->NearPlane(*data);
    }
    
    static void Camera_Get_FarPlane9(Camera* instance, float* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Camera");
        const auto _returnValue = instance->FarPlane();
        *data = _returnValue;
    }

    static void Camera_Set_FarPlane9(Camera* instance, float* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Camera");
        instance->FarPlane(*data);
    }
    
    static void Camera_Get_Forward10(Camera* instance, Vector3* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Camera");
        const auto _returnValue = instance->Forward();
        *data = _returnValue;
    }
    
    static void Camera_Get_Right11(Camera* instance, Vector3* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Camera");
        const auto _returnValue = instance->Right();
        *data = _returnValue;
    }
    
    static void Camera_Get_Up12(Camera* instance, Vector3* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Camera");
        const auto _returnValue = instance->Up();
        *data = _returnValue;
    }
    
    static void Camera_Get_MainCamera13(MonoObject** data) 
    {
        MAIN_THREAD_ONLY();
    }
    
    static Object* Camera_CreateObject(bool createManagedInstance)
    {
        _ASSERT_(createManagedInstance, "Class 'Camera' is not marked as generic, and thus cannot get only unmanaged-instance created!");
        return Object::New<Camera>();
    }
};

void Camera::InitRuntime() 
{
    MONO_REGISTER_OBJECT(&APIProxy::Camera_CreateObject);
    
    MONO_REGISTER_OBJECT_TYPE(Camera);
    API_BIND("ReCrafted.API.Rendering.Camera::InternalSetAsCurrent", &APIProxy::Camera_SetAsCurrent1);
    API_BIND("ReCrafted.API.Rendering.Camera::InternalSetLookAt", &APIProxy::Camera_SetLookAt2);
    API_BIND("ReCrafted.API.Rendering.Camera::InternalCreate", &APIProxy::Camera_Create3);
    API_BIND("ReCrafted.API.Rendering.Camera::Get_InternalBoundingFrustum", &APIProxy::Camera_Get_BoundingFrustum1);
    API_BIND("ReCrafted.API.Rendering.Camera::Get_InternalProjection", &APIProxy::Camera_Get_Projection2);
    API_BIND("ReCrafted.API.Rendering.Camera::Get_InternalView", &APIProxy::Camera_Get_View3);
    API_BIND("ReCrafted.API.Rendering.Camera::Get_InternalViewProjection", &APIProxy::Camera_Get_ViewProjection4);
    API_BIND("ReCrafted.API.Rendering.Camera::Get_InternalPosition", &APIProxy::Camera_Get_Position5);
    API_BIND("ReCrafted.API.Rendering.Camera::Set_InternalPosition", &APIProxy::Camera_Set_Position5);
    API_BIND("ReCrafted.API.Rendering.Camera::Get_InternalRotation", &APIProxy::Camera_Get_Rotation6);
    API_BIND("ReCrafted.API.Rendering.Camera::Set_InternalRotation", &APIProxy::Camera_Set_Rotation6);
    API_BIND("ReCrafted.API.Rendering.Camera::Get_InternalFov", &APIProxy::Camera_Get_Fov7);
    API_BIND("ReCrafted.API.Rendering.Camera::Set_InternalFov", &APIProxy::Camera_Set_Fov7);
    API_BIND("ReCrafted.API.Rendering.Camera::Get_InternalNearPlane", &APIProxy::Camera_Get_NearPlane8);
    API_BIND("ReCrafted.API.Rendering.Camera::Set_InternalNearPlane", &APIProxy::Camera_Set_NearPlane8);
    API_BIND("ReCrafted.API.Rendering.Camera::Get_InternalFarPlane", &APIProxy::Camera_Get_FarPlane9);
    API_BIND("ReCrafted.API.Rendering.Camera::Set_InternalFarPlane", &APIProxy::Camera_Set_FarPlane9);
    API_BIND("ReCrafted.API.Rendering.Camera::Get_InternalForward", &APIProxy::Camera_Get_Forward10);
    API_BIND("ReCrafted.API.Rendering.Camera::Get_InternalRight", &APIProxy::Camera_Get_Right11);
    API_BIND("ReCrafted.API.Rendering.Camera::Get_InternalUp", &APIProxy::Camera_Get_Up12);
    API_BIND("ReCrafted.API.Rendering.Camera::Get_InternalMainCamera", &APIProxy::Camera_Get_MainCamera13);
}

const char* Camera::Fullname() 
{
    return "ReCrafted.API.Rendering.Camera";
}

const char* Camera::Name() 
{
    return "Camera";
}

const char* Camera::Namespace() 
{
    return "ReCrafted.API.Rendering";
}