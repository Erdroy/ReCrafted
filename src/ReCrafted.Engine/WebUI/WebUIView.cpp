// ReCrafted (c) 2016-2019 Always Too Late

#include "WebUIView.h"
#include "WebUIEngine.h"

#include "Core/Action.h"
#include "Common/Text.h"
#include "Common/Display.h"
#include "Common/Profiler/Profiler.h"
#include "Graphics/Graphics.h"
#include "Scripting/Method.h"
#include "WebUI/Impl/UltralightViewport.h"

JSValue WebUIView::ToJSValue(void* data, const MonoTypeEnum type)
{
    switch (type)
    {
    case MONO_TYPE_BOOLEAN: // integer 8
        return JSValue(*static_cast<bool*>(data));
    case MONO_TYPE_CHAR:
        return JSValue(*static_cast<char*>(data));
    case MONO_TYPE_I1:
        return JSValue(*static_cast<sbyte*>(data));
    case MONO_TYPE_U1:
        return JSValue(*static_cast<byte*>(data));

    case MONO_TYPE_I2: // integer 16
        return JSValue(*static_cast<int16_t*>(data));
    case MONO_TYPE_U2:
        return JSValue(*static_cast<uint16_t*>(data));

    case MONO_TYPE_I4: // integer 32
        return JSValue(*static_cast<int32_t*>(data));
    case MONO_TYPE_U4:
        return JSValue(*static_cast<uint32_t*>(data));

    case MONO_TYPE_I8: // integer 64
        return JSValue(*static_cast<int64_t*>(data));
    case MONO_TYPE_U8:
        return JSValue(*static_cast<uint64_t*>(data));

    case MONO_TYPE_R4: // float 32
        return JSValue(*static_cast<float*>(data));

    case MONO_TYPE_R8: // float 64
        return JSValue(*static_cast<double*>(data));

    case MONO_TYPE_STRING:
    {
        cvar monoString = static_cast<MonoString*>(data);
        cvar str = reinterpret_cast<ultralight::Char16*>(mono_string_chars(monoString));
        cvar strLen = mono_string_length(monoString);
        return JSValue(JSString(ultralight::String(str, strLen)));
    }
    default:
        return JSValue();
    }
}

void* WebUIView::FromJSValue(const JSValue& value, MonoType* type)
{
    switch (value.GetType())
    {
    case kJSTypeBoolean:
    case kJSTypeNumber:
    {
        cvar newObjectClass = mono_type_get_class(type);
        cvar newObject = mono_object_new(Domain::Root->GetMono(), newObjectClass);
        cvar newObjectData = mono_object_unbox(newObject);
        cvar newObjectType = static_cast<MonoTypeEnum>(mono_type_get_type(type));

        switch(newObjectType)
        {
        case MONO_TYPE_BOOLEAN: // integer 8
            *static_cast<bool*>(newObjectData) = value.ToBoolean(); break;
        case MONO_TYPE_CHAR:
            *static_cast<char*>(newObjectData) = static_cast<char>(value.ToInteger()); break;
        case MONO_TYPE_I1:
            *static_cast<sbyte*>(newObjectData) = static_cast<sbyte>(value.ToInteger()); break;
        case MONO_TYPE_U1:
            *static_cast<byte*>(newObjectData) = static_cast<byte>(value.ToInteger()); break;

        case MONO_TYPE_I2: // integer 16
            *static_cast<int16_t*>(newObjectData) = static_cast<int16_t>(value.ToInteger()); break;
        case MONO_TYPE_U2:
            *static_cast<uint16_t*>(newObjectData) = static_cast<uint16_t>(value.ToInteger()); break;

        case MONO_TYPE_I4: // integer 32
            *static_cast<int32_t*>(newObjectData) = static_cast<int32_t>(value.ToInteger()); break;
        case MONO_TYPE_U4:
            *static_cast<uint32_t*>(newObjectData) = static_cast<uint32_t>(value.ToInteger()); break;

        case MONO_TYPE_I8: // integer 64
            *static_cast<int64_t*>(newObjectData) = static_cast<int64_t>(value.ToInteger()); break;
        case MONO_TYPE_U8:
            *static_cast<uint64_t*>(newObjectData) = static_cast<uint64_t>(value.ToInteger()); break;

        case MONO_TYPE_R4: // float 32
            *static_cast<float*>(newObjectData) = static_cast<float>(value.ToNumber()); break;

        case MONO_TYPE_R8: // float 64
            *static_cast<double*>(newObjectData) = static_cast<double>(value.ToNumber()); break;

        default:
            break;
        }

        return newObject;
    }
    case kJSTypeString:
    {
        cvar strPtr = const_cast<JSChar*>(value.ToString().GetCharactersPtr());
        cvar monoStrPtr = reinterpret_cast<mono_unichar2*>(strPtr);
        return mono_string_new_utf16(
            Domain::Root->GetMono(),
            monoStrPtr,
            static_cast<int32_t>(wcslen(strPtr))
    );
    }

    case kJSTypeUndefined:
    case kJSTypeNull:
    case kJSTypeObject:
    default:
    {
        _ASSERT_(false, "Got unknown/unsupported type from JavaScript function");
        return nullptr;
    }
    }
}

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
    // TODO: We can use the method definition (parameters) to force cast JSArgs (then use FromJSValue(...))
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

    // Convert result to JSValue
    return ToJSValue(result, resultType);
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

void* WebUIView::Call(const char* functionName, MonoType* returnType, MonoArray* parameters) const
{
    m_viewport->ApplyJSContext();

    cvar js = JSGlobalObject();
    var function = js[functionName].ToFunction();

    // When function is not valid, exit.
    if (!function.IsValid())
        return nullptr;

    // Forward managed arguments to js arguments array
    JSArgs arguments;
    arguments.reserve(8);
    for(var i = 0u; i < mono_array_length(parameters); i ++)
    {
        cvar parameter = mono_array_get(parameters, MonoObject*, i);
        cvar type = mono_object_get_type(parameter);

        if(type == MONO_TYPE_STRING)
            arguments.emplace_back(ToJSValue(parameter, type));
        else
            arguments.emplace_back(ToJSValue(mono_object_unbox(parameter), type));
    }

    // Execute function
    cvar returnData = function(arguments);
    cvar returnDataJSType = returnData.GetType();

    if (!returnType || returnDataJSType == kJSTypeNull || returnDataJSType == kJSTypeUndefined)
        return nullptr;

    return FromJSValue(returnData, returnType);
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
