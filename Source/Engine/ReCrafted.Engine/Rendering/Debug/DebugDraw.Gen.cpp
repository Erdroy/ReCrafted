// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "DebugDraw.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static void DebugDraw_SetColor1(const Color& color) 
    {
        MAIN_THREAD_ONLY();
        DebugDraw::SetColor(color);
    }
    
    static Color DebugDraw_GetColor2() 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = DebugDraw::GetColor();
        return _returnValue;
    }
    
    static void DebugDraw_SetMatrix3(const Matrix& matrix) 
    {
        MAIN_THREAD_ONLY();
        DebugDraw::SetMatrix(matrix);
    }
    
    static Matrix* DebugDraw_GetMatrix4() 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = &DebugDraw::GetMatrix();
        return _returnValue;
    }
    
    static void DebugDraw_DrawArrow5(const Vector3& start, const Vector3& end, float arrowSize) 
    {
        MAIN_THREAD_ONLY();
        DebugDraw::DrawArrow(start, end, arrowSize);
    }
    
    static void DebugDraw_DrawLine6(const Vector3& start, const Vector3& end) 
    {
        MAIN_THREAD_ONLY();
        DebugDraw::DrawLine(start, end);
    }
    
    static void DebugDraw_DrawBox7(const BoundingBox& bounds) 
    {
        MAIN_THREAD_ONLY();
        DebugDraw::DrawBox(bounds);
    }
    
    static void DebugDraw_DrawBox8(const Vector3& center, const Vector3& size) 
    {
        MAIN_THREAD_ONLY();
        DebugDraw::DrawBox(center, size);
    }
    
    static void DebugDraw_DrawWireBox9(const Vector3& center, const Vector3& size) 
    {
        MAIN_THREAD_ONLY();
        DebugDraw::DrawWireBox(center, size);
    }
    
    static void DebugDraw_DrawWireBox10(const BoundingBox& bounds) 
    {
        MAIN_THREAD_ONLY();
        DebugDraw::DrawWireBox(bounds);
    }
    
    static void DebugDraw_DrawWireFrustum11(const BoundingFrustum& frustum) 
    {
        MAIN_THREAD_ONLY();
        DebugDraw::DrawWireFrustum(frustum);
    }
    
    static void DebugDraw_DrawSphere12(const Vector3& center, float radius) 
    {
        MAIN_THREAD_ONLY();
        DebugDraw::DrawSphere(center, radius);
    }
    
    static void DebugDraw_DrawWireSphere13(const Vector3& center, float radius) 
    {
        MAIN_THREAD_ONLY();
        DebugDraw::DrawWireSphere(center, radius);
    }
    
    static void DebugDraw_DrawWireCircle14(const Vector3& center, const Vector3& majorAxis, const Vector3& minorAxis) 
    {
        MAIN_THREAD_ONLY();
        DebugDraw::DrawWireCircle(center, majorAxis, minorAxis);
    }
};

void DebugDraw::InitRuntime() 
{
    MONO_REGISTER_OBJECT_TYPE(DebugDraw);
    API_BIND("ReCrafted.API.Rendering.Debug.DebugDraw::InternalSetColor", &APIProxy::DebugDraw_SetColor1);
    API_BIND("ReCrafted.API.Rendering.Debug.DebugDraw::InternalGetColor", &APIProxy::DebugDraw_GetColor2);
    API_BIND("ReCrafted.API.Rendering.Debug.DebugDraw::InternalSetMatrix", &APIProxy::DebugDraw_SetMatrix3);
    API_BIND("ReCrafted.API.Rendering.Debug.DebugDraw::InternalGetMatrix", &APIProxy::DebugDraw_GetMatrix4);
    API_BIND("ReCrafted.API.Rendering.Debug.DebugDraw::InternalDrawArrow", &APIProxy::DebugDraw_DrawArrow5);
    API_BIND("ReCrafted.API.Rendering.Debug.DebugDraw::InternalDrawLine", &APIProxy::DebugDraw_DrawLine6);
    API_BIND("ReCrafted.API.Rendering.Debug.DebugDraw::InternalDrawBox", &APIProxy::DebugDraw_DrawBox7);
    API_BIND("ReCrafted.API.Rendering.Debug.DebugDraw::InternalDrawBox", &APIProxy::DebugDraw_DrawBox8);
    API_BIND("ReCrafted.API.Rendering.Debug.DebugDraw::InternalDrawWireBox", &APIProxy::DebugDraw_DrawWireBox9);
    API_BIND("ReCrafted.API.Rendering.Debug.DebugDraw::InternalDrawWireBox", &APIProxy::DebugDraw_DrawWireBox10);
    API_BIND("ReCrafted.API.Rendering.Debug.DebugDraw::InternalDrawWireFrustum", &APIProxy::DebugDraw_DrawWireFrustum11);
    API_BIND("ReCrafted.API.Rendering.Debug.DebugDraw::InternalDrawSphere", &APIProxy::DebugDraw_DrawSphere12);
    API_BIND("ReCrafted.API.Rendering.Debug.DebugDraw::InternalDrawWireSphere", &APIProxy::DebugDraw_DrawWireSphere13);
    API_BIND("ReCrafted.API.Rendering.Debug.DebugDraw::InternalDrawWireCircle", &APIProxy::DebugDraw_DrawWireCircle14);
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