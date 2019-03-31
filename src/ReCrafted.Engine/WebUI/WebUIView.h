// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef WEBUIVIEW_H
#define WEBUIVIEW_H

// includes
#include "ReCrafted.h"
#include "Core/Action.h"
#include "Core/Event.h"
#include "Scripting/Object.h"
#include "Scripting/Delegate.h"
#include "WebUI/WebUIViewport.h"
#include "WebUI/Impl/JSHelpers.h"

class WebUIView : public Object
{
    friend class WebUI;

private:
SCRIPTING_API_IMPL()

private:
    int m_width = 0;
    int m_height = 0;
    bool m_fullscreen = false;

    WebUIViewport* m_viewport = nullptr;

    RefPtr<Method> m_apiBeginLoading = nullptr;
    RefPtr<Method> m_apiFinishLoading = nullptr;
    RefPtr<Method> m_apiDOMReady = nullptr;

    spp::sparse_hash_map<JSObjectRef, Delegate> m_callbacks;

private:
    static JSValue ToJSValue(void* data, MonoTypeEnum type);
    static void* FromJSValue(const JSValue& value, MonoType* type);

    JSValue JSCallbackProxy(const JSObject& object, const JSFunction& function, const JSArgs& args);

private:
    void Init(uint width = 0u, uint height = 0u, bool fullscreen = true);
    void Resize(uint width, uint height);
    void Update();
    void Render();
    void RenderView();
    void OnDestroy() override;

public:
    void UpdateSurface(const Vector3& vertex0, const Vector3& vertex1, const Vector3& vertex2, const Vector3& vertex3);

    void Navigate(const char* url);
    void Execute(const char* javaScriptSource);
    void Bind(const char* bindName, const Delegate& callback);
    void* Call(const char* functionName, MonoType* returnType, MonoArray* parameters) const;

public:
    int Width() const { return m_width; }
    int Height() const { return m_height; }
    int IsFullscreen() const { return m_fullscreen; }

public:
    virtual const char* GetObjectName() const override
    {
        return "WebUI View";
    }

public:
    Event<>& BeginLoading() const;
    Event<>& FinishLoading() const;
    Event<>& DOMReady() const;

public:
    PROPERTY(bool, Active) = true;
};

#endif // WEBUIVIEW_H
