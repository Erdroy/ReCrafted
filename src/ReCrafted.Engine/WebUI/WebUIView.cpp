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

JSValue WebUIView::JSCallbackProxy(const JSObject& object, const JSFunction& function, const JSArgs& args)
{
    ASSERT(args.size() <= 4u); // Only 4 arguments are supported

    // Find function
    cvar it = m_callbacks.find(function);

    // No function found, return empty value.
    if (it == m_callbacks.end())
        return JSValue();

    crvar callback = it->second;
    void* params[4] = { nullptr };

    // Forward parameters
    // TODO: We can use the method definition (parameters) to force cast JSArgs
    for(var i = 0u; i < args.size() && i < 4; i ++)
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

    // Invoke mono callback function
    MonoTypeEnum resultType;
    cvar result = callback.Invoke(args.empty() ? nullptr : params, reinterpret_cast<int*>(&resultType));

    // No result, return empty value.
    if(result == nullptr)
        return JSValue();

    switch(resultType)
    {
    case MONO_TYPE_BOOLEAN: // integer 8
        return JSValue(*static_cast<bool*>(result));
    case MONO_TYPE_CHAR:
        return JSValue(*static_cast<char*>(result));
    case MONO_TYPE_I1:
        return JSValue(*static_cast<sbyte*>(result));
    case MONO_TYPE_U1:
        return JSValue(*static_cast<byte*>(result));

    case MONO_TYPE_I2: // integer 16
        return JSValue(*static_cast<int16_t*>(result));
    case MONO_TYPE_U2:
        return JSValue(*static_cast<uint16_t*>(result));

    case MONO_TYPE_I4: // integer 32
        return JSValue(*static_cast<int32_t*>(result));
    case MONO_TYPE_U4:
        return JSValue(*static_cast<uint32_t*>(result));

    case MONO_TYPE_I8: // integer 64
        return JSValue(*static_cast<int64_t*>(result));
    case MONO_TYPE_U8:
        return JSValue(*static_cast<uint64_t*>(result));

    case MONO_TYPE_R4: // float 32
        return JSValue(*static_cast<float*>(result));

    case MONO_TYPE_R8: // float 64
        return JSValue(*static_cast<double*>(result));

    case MONO_TYPE_STRING:
    {
        cvar monoString = static_cast<MonoString*>(result);
        cvar str = reinterpret_cast<ultralight::Char16*>(mono_string_chars(monoString));
        cvar strLen = mono_string_length(monoString);
        return JSValue(JSString(ultralight::String(str, strLen)));
    }
    default:
        return JSValue();
    }
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

    cvar function = js[bindName].ToFunction();

    // Insert callback
    m_callbacks.insert(std::make_pair(static_cast<JSObjectRef>(function), callback));
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
