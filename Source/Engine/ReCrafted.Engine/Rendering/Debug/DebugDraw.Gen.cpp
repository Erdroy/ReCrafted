// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "DebugDraw.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static void DebugDraw_DrawArrow1(const Vector3& start, const Vector3& end, float arrowSize) 
    {
        MAIN_THREAD_ONLY();
        DebugDraw::DrawArrow(start, end, arrowSize);
    }
    
    static void DebugDraw_DrawLine2(const Vector3& start, const Vector3& end) 
    {
        MAIN_THREAD_ONLY();
        DebugDraw::DrawLine(start, end);
    }
    
    static void DebugDraw_DrawBox3(const BoundingBox& bounds) 
    {
        MAIN_THREAD_ONLY();
        DebugDraw::DrawBox(bounds);
    }
    
    static void DebugDraw_DrawBox4(const Vector3& center, const Vector3& size) 
    {
        MAIN_THREAD_ONLY();
        DebugDraw::DrawBox(center, size);
    }
    
    static void DebugDraw_DrawWireBox5(const Vector3& center, const Vector3& size) 
    {
        MAIN_THREAD_ONLY();
        DebugDraw::DrawWireBox(center, size);
    }
    
    static void DebugDraw_DrawWireBox6(const BoundingBox& bounds) 
    {
        MAIN_THREAD_ONLY();
        DebugDraw::DrawWireBox(bounds);
    }
    
    static void DebugDraw_DrawWireFrustum7(const BoundingFrustum& frustum) 
    {
        MAIN_THREAD_ONLY();
        DebugDraw::DrawWireFrustum(frustum);
    }
    
    static void DebugDraw_DrawSphere8(const Vector3& center, float radius) 
    {
        MAIN_THREAD_ONLY();
        DebugDraw::DrawSphere(center, radius);
    }
    
    static void DebugDraw_DrawWireSphere9(const Vector3& center, float radius) 
    {
        MAIN_THREAD_ONLY();
        DebugDraw::DrawWireSphere(center, radius);
    }
    
    static void DebugDraw_DrawWireCircle10(const Vector3& center, const Vector3& majorAxis, const Vector3& minorAxis) 
    {
        MAIN_THREAD_ONLY();
        DebugDraw::DrawWireCircle(center, majorAxis, minorAxis);
    }
    
    static void DebugDraw_Get_Color1(Color4* data) 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = DebugDraw::GetColor();
        *data = _returnValue;
    }

    static void DebugDraw_Set_Color1(Color4* data) 
    {
        MAIN_THREAD_ONLY();
        DebugDraw::SetColor(*data);
    }
    
    static void DebugDraw_Get_Matrix2(Matrix* data) 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = DebugDraw::GetMatrix();
        *data = _returnValue;
    }

    static void DebugDraw_Set_Matrix2(Matrix* data) 
    {
        MAIN_THREAD_ONLY();
        DebugDraw::SetMatrix(*data);
    }
};

void DebugDraw::InitRuntime() 
{
    MONO_REGISTER_OBJECT_TYPE(DebugDraw);
    API_BIND("ReCrafted.API.Rendering.Debug.DebugDraw::InternalDrawArrow", &APIProxy::DebugDraw_DrawArrow1);
    API_BIND("ReCrafted.API.Rendering.Debug.DebugDraw::InternalDrawLine", &APIProxy::DebugDraw_DrawLine2);
    API_BIND("ReCrafted.API.Rendering.Debug.DebugDraw::InternalDrawBox", &APIProxy::DebugDraw_DrawBox3);
    API_BIND("ReCrafted.API.Rendering.Debug.DebugDraw::InternalDrawBox", &APIProxy::DebugDraw_DrawBox4);
    API_BIND("ReCrafted.API.Rendering.Debug.DebugDraw::InternalDrawWireBox", &APIProxy::DebugDraw_DrawWireBox5);
    API_BIND("ReCrafted.API.Rendering.Debug.DebugDraw::InternalDrawWireBox", &APIProxy::DebugDraw_DrawWireBox6);
    API_BIND("ReCrafted.API.Rendering.Debug.DebugDraw::InternalDrawWireFrustum", &APIProxy::DebugDraw_DrawWireFrustum7);
    API_BIND("ReCrafted.API.Rendering.Debug.DebugDraw::InternalDrawSphere", &APIProxy::DebugDraw_DrawSphere8);
    API_BIND("ReCrafted.API.Rendering.Debug.DebugDraw::InternalDrawWireSphere", &APIProxy::DebugDraw_DrawWireSphere9);
    API_BIND("ReCrafted.API.Rendering.Debug.DebugDraw::InternalDrawWireCircle", &APIProxy::DebugDraw_DrawWireCircle10);
    API_BIND("ReCrafted.API.Rendering.Debug.DebugDraw::Get_InternalColor", &APIProxy::DebugDraw_Get_Color1);
    API_BIND("ReCrafted.API.Rendering.Debug.DebugDraw::Set_InternalColor", &APIProxy::DebugDraw_Set_Color1);
    API_BIND("ReCrafted.API.Rendering.Debug.DebugDraw::Get_InternalMatrix", &APIProxy::DebugDraw_Get_Matrix2);
    API_BIND("ReCrafted.API.Rendering.Debug.DebugDraw::Set_InternalMatrix", &APIProxy::DebugDraw_Set_Matrix2);
}

const char* DebugDraw::Fullname() 
{
    return "ReCrafted.API.Rendering.Debug.DebugDraw";
}

const char* DebugDraw::Name() 
{
    return "DebugDraw";
}

const char* DebugDraw::Namespace() 
{
    return "ReCrafted.API.Rendering.Debug";
}