// ReCrafted (c) 2016-2019 Always Too Late

#include "UltralightViewport.h"
#include "Core/Logger.h"
#include "Common/Profiler/Profiler.h"
#include "Input/InputManager.h"
#include "Renderer/RHI/RHIContext.h"

#include "d3d11/GPUDriverD3D11.h"

#include <Ultralight/platform/Platform.h>
#include <Ultralight/platform/GPUDriver.h>

void UltralightViewport::CreateTexture(const uint width, const uint height)
{
    // Create texture
    m_texture = Renderer::CreateTexture2D(width, height, 0, 0, Renderer::TextureFormat::BGRA8, nullptr, 0u);
}

void UltralightViewport::DestroyTexture()
{
    Renderer::DestroyTexture2D(m_texture);
    m_texture = {};
}

UltralightViewport::UltralightViewport(const int width, const int height, const ultralight::Ref<ultralight::View>& view)
{
    m_view = view;

    m_view->set_load_listener(this);
    m_view->set_view_listener(this);

    // Create texture
    CreateTexture(width, height);
    m_skipFrames++;
}

UltralightViewport::~UltralightViewport()
{
    DestroyTexture();
}

void UltralightViewport::Update()
{
    CPU_PROFILE_SCOPE(0, __FUNCTION__);

    rvar frameInput = InputManager::GetInstance()->GetFrameInput();

    for(crvar input : frameInput)
    {
        switch(input.first)
        {
        case InputType::Key:
        {
            ultralight::KeyEvent event{};
            ultralight::GetKeyIdentifierFromVirtualKeyCode(static_cast<int>(input.second.key), event.key_identifier);
            event.native_key_code = static_cast<int>(input.second.key);
            event.virtual_key_code = static_cast<int>(input.second.key);

            m_view->FireKeyEvent(event);
            break;
        }
        case InputType::Character:
        {
            ultralight::KeyEvent event{};
            event.type = ultralight::KeyEvent::Type::kType_Char;
            event.text = ultralight::String((ultralight::Char16*)&input.second.character, 1u);
            m_view->FireKeyEvent(event);
            break;
        }
        case InputType::Button:
        {
            ultralight::MouseEvent event{};
            event.type = input.second.buttonState == ButtonState::Down ? 
                ultralight::MouseEvent::kType_MouseDown : 
                ultralight::MouseEvent::kType_MouseUp;
            event.x = static_cast<int>(ceilf(InputManager::GetCursorPosition().x));
            event.y = static_cast<int>(ceilf(InputManager::GetCursorPosition().y));
            event.button = static_cast<ultralight::MouseEvent::Button>(static_cast<int>(input.second.button));

            m_view->FireMouseEvent(event);
            break;
        }
        case InputType::Cursor:
        {
            ultralight::MouseEvent event{};
            event.type = ultralight::MouseEvent::kType_MouseMoved;
            event.x = static_cast<int>(ceilf(input.second.axis2D.x));
            event.y = static_cast<int>(ceilf(input.second.axis2D.y));
            event.button = ultralight::MouseEvent::kButton_None;

            m_view->FireMouseEvent(event);
            break;
        }
        case InputType::Scroll:
        {
            ultralight::ScrollEvent event{};
            event.type = ultralight::ScrollEvent::kType_ScrollByPixel;
            event.delta_y = static_cast<int>(input.second.axis1D * 120); // TODO: Smooth scroll implementation

            m_view->FireScrollEvent(event);
            break;
        }

        case InputType::Unknown:
        case InputType::Axis:
        case InputType::Count:
        default: break;
        }
    }
}

void UltralightViewport::Render()
{
    CPU_PROFILE_SCOPE(0, __FUNCTION__);

    if(m_skipFrames > 0)
    {
        m_skipFrames--;
        return;
    }

    cvar renderTarget = m_view->render_target();
    if(!renderTarget.is_empty)
    {
        cvar driver = (ultralight::GPUDriverD3D11*)ultralight::Platform::instance().gpu_driver();
        cvar sourceTexture = driver->GetTexture(m_view->render_target().texture_id);

        Renderer::CopyTextureSubresourceSync(m_texture, sourceTexture);
    }
}

void UltralightViewport::Resize(const uint width, const uint height)
{
    // Recreate texture
    DestroyTexture();
    CreateTexture(width, height);

    m_view->Resize(width, height);
    m_skipFrames++;
}

void UltralightViewport::Navigate(const char* url)
{
    m_view->LoadURL(url);
}

void UltralightViewport::Execute(const char* javaScriptSource)
{
    m_view->EvaluateScript(javaScriptSource);
}

void UltralightViewport::OnChangeCursor(ultralight::View* caller, ultralight::Cursor cursor)
{
    // TODO: Cursor change implementation
}

void UltralightViewport::OnAddConsoleMessage(ultralight::View* caller, ultralight::MessageSource source,
    ultralight::MessageLevel level, const ultralight::String& message, uint32_t line_number, uint32_t column_number,
    const ultralight::String& source_id)
{
    switch (level)
    {
    case ultralight::kMessageLevel_Debug:
    case ultralight::kMessageLevel_Info:
    case ultralight::kMessageLevel_Log: 
        Logger::Log("{0}({1}): {2}", source_id.utf8().data(), line_number, message.utf8().data()); 
        break;
    case ultralight::kMessageLevel_Warning: 
        Logger::LogWarning("{0}({1}): {2}", source_id.utf8().data(), line_number, message.utf8().data()); 
        break;
    case ultralight::kMessageLevel_Error: 
        Logger::LogError("{0}({1}): {2}", source_id.utf8().data(), line_number, message.utf8().data()); 
        break;
    default: 
        break;
    }
}

void UltralightViewport::OnBeginLoading(ultralight::View* caller)
{
    m_onBeginLoading.Invoke();
}

void UltralightViewport::OnFinishLoading(ultralight::View* caller)
{
    m_onFinishLoading.Invoke();
}

void UltralightViewport::OnDOMReady(ultralight::View* caller)
{
    m_onDOMReady.Invoke();
}
