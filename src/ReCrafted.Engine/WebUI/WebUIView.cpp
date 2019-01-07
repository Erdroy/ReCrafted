// ReCrafted (c) 2016-2019 Always Too Late

#include "WebUIView.h"
#include "WebUIEngine.h"

#include "Core/Action.h"
#include "Common/Text.h"
#include "Common/Display.h"
#include "Common/Profiler/Profiler.h"
#include "Graphics/Graphics.h"
#include "WebUI/Impl/UltralightViewport.h"
#include "Scripting/Method.h"

#include <string>

JSValue WebUIView::JSCallbackProxy(const JSObject& object, const JSFunction& function, const JSArgs& args)
{
    // Find function
    cvar functionName = std::wstring(function.GetName().GetCharactersPtr());
    cvar it = m_callbacks.find(functionName);

    // No function found, return empty value.
    if (it == m_callbacks.end())
        return JSValue();

    crvar callback = it->second;
    void* params[8] = { nullptr };

    // Forward parameters
    for(var i = 0u; i < args.size(); i ++)
    {
        rvar arg = args[i];
        cvar type = arg.GetType();
        switch(type)
        {
        case kJSTypeBoolean:
        {
            // Create boolean parameter
            cvar number = arg.ToBoolean();
            params[i] = alloca(sizeof(bool));
            *static_cast<bool*>(params[i]) = number;
            continue;
        }

        case kJSTypeNumber:
        {
            // Create number (float) parameter
            cvar number = arg.ToNumber();
            params[i] = alloca(sizeof(float));
            *static_cast<float*>(params[i]) = static_cast<float>(number);
            continue;
        }

        case kJSTypeString:
        {
            // Create string parameter
            cvar strPtr = const_cast<JSChar*>(arg.ToString().GetCharactersPtr());
            cvar monoStrPtr = reinterpret_cast<mono_unichar2*>(strPtr);
            params[i] = mono_string_new_utf16(
                Domain::Root->GetMono(),
                monoStrPtr,
                static_cast<int32_t>(wcslen(strPtr))
            );
            continue;
        }
        default: 
            continue;
        }
    }

    void** paramPtr = nullptr;
    if (!args.empty())
        paramPtr = params;
    cvar result = callback.Invoke(paramPtr);

    // No result, return empty value.
    if(result == nullptr)
        return JSValue();

    // TODO: Function to find out what return type the function should have... But how (using mono?)?
    // TODO: Forward return parameter
    return JSValue();
}

void WebUIView::Init(const uint width, const uint height, const bool fullscreen)
{
    m_fullscreen = fullscreen;
    m_width = fullscreen ? Display::GetWidth() : width;
    m_height = fullscreen ? Display::GetHeight() : height;

    m_viewport = WebUIEngine::CreateUIViewport(this, fullscreen);
    ASSERT(m_viewport);

    // Initialize internal API calls
    m_apiBeginLoading = FindMethod("ReCrafted.API.WebUI::OnBeginLoading");
    m_apiFinishLoading = FindMethod("ReCrafted.API.WebUI::OnFinishLoading");
    m_apiDOMReady = FindMethod("ReCrafted.API.WebUI::OnDOMReady");

    ASSERT(m_apiBeginLoading);
    ASSERT(m_apiFinishLoading);
    ASSERT(m_apiDOMReady);

    BeginLoading().AddListener(Action<void>([this]()
        {
            m_apiBeginLoading->Invoke();
        }));

    FinishLoading().AddListener(Action<void>([this]()
        {
            m_apiFinishLoading->Invoke();
        }));

    DOMReady().AddListener(Action<void>([this]()
        {
            m_apiDOMReady->Invoke();
        }));
}

void WebUIView::Resize(const uint width, const uint height)
{
    ASSERT(m_viewport);

    m_viewport->Resize(width, height);
    m_width = width;
    m_height = height;
}

void WebUIView::Update()
{
    ASSERT(m_viewport);

    m_viewport->Update();
}

void WebUIView::Render()
{
    Profiler::BeginProfile(__FUNCTION__);

    if (!m_Active)
        return;

    ASSERT(m_viewport);

    m_viewport->Render();

    Profiler::EndProfile();
}

void WebUIView::RenderView()
{
    Profiler::BeginProfile(__FUNCTION__);

    if (!m_Active)
        return;

    if (!m_fullscreen)
    {
        // TODO: Render world-space quad
    }
    else
    {
        // Blit into back-buffer
        Renderer::BlitTexture(Graphics::GetInstance()->GetFrameBuffer(), m_viewport->GetTexture());
    }

    Profiler::EndProfile();
}

void WebUIView::OnDestroy()
{
    SafeDelete(m_viewport);
}

void WebUIView::UpdateSurface(const Vector3& vertex0, const Vector3& vertex1, const Vector3& vertex2,
    const Vector3& vertex3)
{
    // TODO: Build world space quad
}

void WebUIView::Navigate(const char* url)
{
    ASSERT(m_viewport);
    m_viewport->Navigate(url);
}

void WebUIView::Execute(const char* javaScriptSource)
{
    ASSERT(m_viewport);
    m_viewport->Execute(javaScriptSource);
}

void WebUIView::Bind(const char* bindName, const Delegate& callback)
{
    m_viewport->ApplyJSContext();

    cvar js = JSGlobalObject();
    js[bindName] = BindJSCallbackWithRetval(&WebUIView::JSCallbackProxy);

    var function = js[bindName].ToFunction();

    // Insert callback
    m_callbacks.insert(std::make_pair(std::wstring(function.GetName().GetCharactersPtr()), callback));
}

Event<>& WebUIView::BeginLoading() const
{
    return static_cast<UltralightViewport*>(m_viewport)->BeginLoading();
}

Event<>& WebUIView::FinishLoading() const
{
    return static_cast<UltralightViewport*>(m_viewport)->FinishLoading();
}

Event<>& WebUIView::DOMReady() const
{
    return static_cast<UltralightViewport*>(m_viewport)->DOMReady();
}
