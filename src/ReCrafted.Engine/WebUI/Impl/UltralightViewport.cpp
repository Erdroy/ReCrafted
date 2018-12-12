// ReCrafted (c) 2016-2018 Always Too Late

#include "UltralightViewport.h"
#include "Common/Profiler/Profiler.h"
#include "Common/Input/Input.h"
#include <Ultralight/platform/Platform.h>
#include <Ultralight/platform/GPUDriver.h>
#include "d3d11/GPUDriverD3D11.h"
#include "Renderer/RHI/RHIContext.h"

void UltralightViewport::CreateTexture(const uint width, const uint height)
{
    // Create texture
    m_texture = Renderer::CreateTexture2D(width, height, 0, 0, Renderer::TextureFormat::RGBA8, nullptr, 0u);
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
}

UltralightViewport::~UltralightViewport()
{
    DestroyTexture();
}

void UltralightViewport::Update()
{
    ultralight::MouseEvent event;
    event.type = ultralight::MouseEvent::kType_MouseMoved;
    event.x = ceilf(Input::GetCursorPos().x);
    event.y = ceilf(Input::GetCursorPos().y);

    m_view->FireMouseEvent(event);

    if(Input::IsKeyDown(Key_Mouse0))
    {
        ultralight::MouseEvent event;
        event.type = ultralight::MouseEvent::kType_MouseDown;
        event.button = ultralight::MouseEvent::kButton_Left;
        event.x = ceilf(Input::GetCursorPos().x);
        event.y = ceilf(Input::GetCursorPos().y);

        m_view->FireMouseEvent(event);
    }

    if (Input::IsKeyUp(Key_Mouse0))
    {
        ultralight::MouseEvent event;
        event.type = ultralight::MouseEvent::kType_MouseUp;
        event.button = ultralight::MouseEvent::kButton_Left;
        event.x = ceilf(Input::GetCursorPos().x);
        event.y = ceilf(Input::GetCursorPos().y);

        m_view->FireMouseEvent(event);
    }
}

void UltralightViewport::Render()
{
    Profiler::BeginProfile(__FUNCTION__);

    cvar renderTarget = m_view->render_target();
    if(!renderTarget.is_empty)
    {
        cvar driver = (ultralight::GPUDriverD3D11*)ultralight::Platform::instance().gpu_driver();
        cvar sourceTexture = driver->GetTexture(m_view->render_target().texture_id);

        // Copy sourceTexture to m_texture

        //Renderer::CopyTextureSubresourceSync()
    }

    Profiler::EndProfile();
}

void UltralightViewport::Resize(const uint width, const uint height)
{
    // Recreate texture
    DestroyTexture();
    CreateTexture(width, height);

    m_view->Resize(width, height);
}

void UltralightViewport::Navigate(const char* url)
{
    m_view->LoadURL(url);
}

void UltralightViewport::Execute(const char* javaScriptSource)
{
    m_view->EvaluateScript(javaScriptSource);
}

void UltralightViewport::OnChangeTitle(ultralight::View* caller, const ultralight::String& title)
{
}

void UltralightViewport::OnChangeURL(ultralight::View* caller, const ultralight::String& url)
{
}

void UltralightViewport::OnChangeTooltip(ultralight::View* caller, const ultralight::String& tooltip)
{
}

void UltralightViewport::OnChangeCursor(ultralight::View* caller, ultralight::Cursor cursor)
{
}

void UltralightViewport::OnAddConsoleMessage(ultralight::View* caller, ultralight::MessageSource source,
    ultralight::MessageLevel level, const ultralight::String& message, uint32_t line_number, uint32_t column_number,
    const ultralight::String& source_id)
{
}

void UltralightViewport::OnBeginLoading(ultralight::View* caller)
{
}

void UltralightViewport::OnFinishLoading(ultralight::View* caller)
{
}

void UltralightViewport::OnUpdateHistory(ultralight::View* caller)
{
}

void UltralightViewport::OnDOMReady(ultralight::View* caller)
{
}
