// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "WebUIView.h"
#include "Profiler/Profiler.h"
#include "Input/InputManager.h"
#include "Impl/d3d11/GPUDriverD3D11.h"
#include "Impl/JSHelpers.h"
#include "Common/Logger.h"
#include "WebUIManager.h"
#include "Rendering/RenderingManager.h"

void WebUIView::CreateTexture(const uint width, const uint height)
{
    // Create texture
    m_texture = Renderer::CreateTexture2D(width, height, 0, 0, Renderer::TextureFormat::BGRA8, nullptr, 0u);
}

void WebUIView::DestroyTexture()
{
    Renderer::DestroyTexture2D(m_texture);
    m_texture = {};
}

WebUIView::~WebUIView()
{
    // Remove this view from the views list
    if(!WebUIManager::IsReleased())
        WebUIManager::GetInstance()->m_views.Remove(this);

    m_view = nullptr;

    Logger::LogWarning("WebUIView '{0}' released", m_name);
}

void WebUIView::Initialize(const int width, const int height, const ultralight::RefPtr<ultralight::View>& view)
{
    // Set initial name
    //m_name = STRING_CONST("Unnamed");

    // Set ultralight's view instance
    m_view = view;

    // Set view listeners
    m_view->set_load_listener(this);
    m_view->set_view_listener(this);

    // Create texture
    CreateTexture(width, height);

    // Skip next frame
    m_skipFrames = 2;

    // Navigate to the default page
    //Navigate("file:\\Default.html");
}

void WebUIView::Update()
{
    if (!m_active)
        return;

    CPU_PROFILE_SCOPE(0, __FUNCTION__);

    auto& frameInput = InputManager::GetInstance()->GetFrameInput();

    for (const auto& input : frameInput)
    {
        switch (input.first)
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

void WebUIView::Render()
{
    if (!m_active)
        return;

    CPU_PROFILE_SCOPE(0, __FUNCTION__);

    if (m_resize)
    {
        // Resize view and skip next frame
        m_view->Resize(m_width, m_height);
        m_resize = false;
    }

    if (m_skipFrames > 0)
    {
        m_skipFrames--;
        return;
    }

    const auto renderTarget = m_view->render_target();
    if (!renderTarget.is_empty)
    {
        const auto driver = static_cast<ultralight::GPUDriverD3D11*>(ultralight::Platform::instance().gpu_driver());
        const auto sourceTexture = driver->GetTexture(renderTarget.texture_id);

        if(sourceTexture != nullptr)
        {
            // Copy texture data
            Renderer::CopyTextureSubresourceSync(m_texture, sourceTexture);
        }
        else
        {
            Logger::LogWarning("WebUIView '{0}' of size {1}x{2} has no render texture, force resizing.", m_name, m_width, m_height);
            m_resize = true;
        }
    }
}

void WebUIView::RenderFullscreen() const
{
    if (!m_active)
        return;

    CPU_PROFILE_FUNCTION(0);

    // Blit into back-buffer
    Renderer::BlitTexture(RenderingManager::GetFrameBuffer(), GetTexture());
}

void WebUIView::Resize(const uint width, const uint height)
{
    if (m_width == static_cast<int>(width) && m_height == static_cast<int>(height))
        return;

    // Recreate texture
    DestroyTexture();
    CreateTexture(width, height);

    // Queue resize.
    m_resize = true;
    m_skipFrames = 1;

    // Cache new width and height
    m_width = width;
    m_height = height;
}

void WebUIView::Navigate(const char* url) const
{
    m_view->LoadURL(url);
}

void WebUIView::Execute(const char* javaScriptSource) const
{
    m_view->EvaluateScript(javaScriptSource);
}

void WebUIView::ApplyJSContext() const
{
    const auto myContext = m_view->js_context();

    // Set the JSContext for all subsequent JSHelper calls
    SetJSContext(myContext);
}

void WebUIView::SetActive(const bool isActive)
{
    m_active = isActive;
}

void WebUIView::OnChangeCursor(ultralight::View* caller, ultralight::Cursor cursor)
{
    // TODO: Cursor change implementation
}

void WebUIView::OnAddConsoleMessage(ultralight::View* caller, ultralight::MessageSource source,
                                    const ultralight::MessageLevel level, const ultralight::String& message,
                                    const uint32_t line_number, const uint32_t column_number,
                                    const ultralight::String& source_id)
{
    switch (level)
    {
    case ultralight::kMessageLevel_Debug:
    case ultralight::kMessageLevel_Info:
    case ultralight::kMessageLevel_Log:
        Logger::Log("{0}(line {1}, col {2}): {3}", source_id.utf8().data(), line_number, column_number, message.utf8().data());
        break;
    case ultralight::kMessageLevel_Warning:
        Logger::LogWarning("{0}(line {1}, col {2}): {3}", source_id.utf8().data(), line_number, column_number, message.utf8().data());
        break;
    case ultralight::kMessageLevel_Error:
        Logger::LogError("{0}(line {1}, col {2}): {3}", source_id.utf8().data(), line_number, column_number, message.utf8().data());
        break;
    default:
        break;
    }
}

void WebUIView::OnBeginLoading(ultralight::View* caller)
{
    m_onBeginLoading.Invoke();
}

void WebUIView::OnFinishLoading(ultralight::View* caller)
{
    m_onFinishLoading.Invoke();
}

void WebUIView::OnDOMReady(ultralight::View* caller)
{
    m_onDOMReady.Invoke();
}
