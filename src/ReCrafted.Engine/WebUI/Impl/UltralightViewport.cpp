// ReCrafted (c) 2016-2018 Always Too Late

#include "UltralightViewport.h"
#include "Common/Profiler/Profiler.h"

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

    m_view->LoadHTML("<h1 style='color: rgb(255, 10, 0);'>Hello, World!</h1>");

    // Create texture
    CreateTexture(width, height);
}

UltralightViewport::~UltralightViewport()
{
    DestroyTexture();
}

void UltralightViewport::Update()
{
}

void UltralightViewport::Render()
{
    Profiler::BeginProfile(__FUNCTION__);

    if(m_view->is_bitmap_dirty())
    {
        cvar bitmap = m_view->bitmap();
        cvar size = bitmap->size();

        // Allocate new texture data and copy the bitmap contents into it
        cvar newTextureData = Renderer::Allocate(size);

        // Copy bitmap contents
        bitmap->LockPixels();
        memcpy(newTextureData, bitmap->raw_pixels(), size);
        bitmap->UnlockPixels();

        // Send renderer command
        Renderer::UpdateTextureSubresource(m_texture, newTextureData, size);
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
