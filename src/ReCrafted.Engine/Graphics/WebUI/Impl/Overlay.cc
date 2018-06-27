#include "Overlay.h"

#include <Ultralight/Renderer.h>
#include <Ultralight/platform/Platform.h>
#include <Ultralight/platform/Config.h>

#include "D3DRenderer.h"

static ultralight::IndexType patternCW[] = { 0, 1, 3, 1, 2, 3 };
static ultralight::IndexType patternCCW[] = { 0, 3, 1, 1, 3, 2 };

Overlay::Overlay(bool fullscreen, ultralight::Renderer* renderer, GPUDriver* driver, int width, int height) :
    m_fullscreen(fullscreen), 
    m_width(width), 
    m_height(height),
    m_view(renderer->CreateView(width, height, true)),
    m_driver(driver)
{
    m_texture = driver->NextTextureId();

    const auto bitmap = Bitmap::Create(width, height, kBitmapFormat_RGBA8, 0, nullptr, 0, false);
    driver->CreateTexture(m_texture, bitmap);

    RenderBuffer buffer = {};
    buffer.width = width;
    buffer.height = height;
    buffer.has_depth_buffer = false;
    buffer.has_stencil_buffer = false;
    buffer.texture_id = m_texture;

    m_renderBuffer = driver->NextRenderBufferId();
    driver->CreateRenderBuffer(m_renderBuffer, buffer);
}

Overlay::~Overlay() 
{
    if (!m_vertices.empty())
        m_driver->DestroyGeometry(m_geometryId);
}

void Overlay::Resize(uint32_t width, uint32_t height)
{
    m_width = width;
    m_height = height;
}

void Overlay::Draw() 
{
    D3DRenderer::GetInstance()->SetViewportSize(m_width, m_height);

    m_driver->BindRenderBuffer(m_fullscreen ? 0 : m_renderBuffer);
   
    UpdateGeometry();
    
    m_gpuState.texture_1_id = view()->render_target().texture_id;
    m_driver->DrawGeometry(m_geometryId, 6, 0, m_gpuState);
}

void Overlay::KeyDown(WPARAM wparam, LPARAM lparam, bool is_system_key) const
{
    const KeyEvent evt(KeyEvent::kType_RawKeyDown, wparam, lparam, is_system_key);
    view()->FireKeyEvent(evt);
}

void Overlay::KeyUp(WPARAM wparam, LPARAM lparam, bool is_system_key) const
{
    const KeyEvent evt(KeyEvent::kType_KeyUp, wparam, lparam, is_system_key);
    view()->FireKeyEvent(evt);
}

void Overlay::CharPress(WPARAM wparam, LPARAM lparam, bool is_system_key) const
{
    const KeyEvent evt(KeyEvent::kType_Char, wparam, lparam, is_system_key);
    view()->FireKeyEvent(evt);
}

void Overlay::MouseMove(int x, int y) const
{
    MouseEvent evt;
    evt.type = MouseEvent::kType_MouseMoved;
    evt.x = x;
    evt.y = y;
    evt.button = MouseEvent::kButton_None;

    view()->FireMouseEvent(evt);
}

MouseEvent::Button ToButton(MouseButton button) 
{
    switch (button) {
    case MouseButton::kMouseButton_Left: return MouseEvent::Button::kButton_Left;
    case MouseButton::kMouseButton_Middle: return MouseEvent::Button::kButton_Middle;
    case MouseButton::kMouseButton_Right: return MouseEvent::Button::kButton_Right;
    default: return MouseEvent::Button::kButton_None;
    }
}

void Overlay::MouseButtonDown(MouseButton button, int x, int y) const
{
    MouseEvent evt;
    evt.type = MouseEvent::kType_MouseDown;
    evt.x = x;
    evt.y = y;
    evt.button = ToButton(button);

    view()->FireMouseEvent(evt);
}

void Overlay::MouseButtonUp(MouseButton button, int x, int y) const
{
    MouseEvent evt;
    evt.type = MouseEvent::kType_MouseUp;
    evt.x = x;
    evt.y = y;
    evt.button = ToButton(button);

    view()->FireMouseEvent(evt);
}

void Overlay::MouseWheel(int delta) const
{
    ScrollEvent evt;
    evt.delta_x = 0;
    evt.delta_y = delta;
    evt.type = ScrollEvent::kType_ScrollByPixel;

    view()->FireScrollEvent(evt);
}

void Overlay::OnFinishLoading(ultralight::View* caller)
{
}

void Overlay::OnBeginLoading(ultralight::View* caller)
{
}

void Overlay::OnUpdateHistory(ultralight::View* caller)
{
}

void Overlay::OnDOMReady(ultralight::View* caller)
{
}

void Overlay::OnAddConsoleMessage(ultralight::View* caller, MessageSource source, MessageLevel level,
    const String& message, uint32_t line_number, uint32_t column_number, const String& source_id)
{
    //OutputDebugStringA((const char*)message.utf8().data());
}

void Overlay::UpdateGeometry() {
    bool initial_creation = false;
    if (m_vertices.empty()) {
        m_vertices.resize(4);
        m_indices.resize(6);

        auto& config = ultralight::Platform::instance().config();
        if (config.face_winding == kFaceWinding_Clockwise)
            memcpy(m_indices.data(), patternCW, sizeof(IndexType) * m_indices.size());
        else
            memcpy(m_indices.data(), patternCCW, sizeof(IndexType) * m_indices.size());

        memset(&m_gpuState, 0, sizeof(m_gpuState));
        ultralight::Matrix identity{};
        identity.SetIdentity();
        m_gpuState.transform = ConvertAffineTo4x4(identity);
        m_gpuState.enable_blend = true;
        m_gpuState.enable_texturing = true;
        m_gpuState.shader_type = kShaderType_Fill;
        m_gpuState.render_buffer_id = 0; // default render target view (screen)

        initial_creation = true;
    }

    if (!m_needsUpdate)
        return;

    Vertex_2f_4ub_2f_2f_28f surface {};
    surface.data0[0] = 1; // Fill Type: Image

    surface.color[0] = 255;
    surface.color[1] = 255;
    surface.color[2] = 255;
    surface.color[3] = 255;

    const auto left = 0.0f;
    const auto top = 0.0f;
    const auto right = 0.0f + width();
    const auto bottom = 0.0f + height();

    // TOP LEFT
    surface.pos[0] = surface.obj[0] = left;
    surface.pos[1] = surface.obj[1] = top;
    surface.tex[0] = 0;
    surface.tex[1] = 0;

    m_vertices[0] = surface;

    // TOP RIGHT
    surface.pos[0] = surface.obj[0] = right;
    surface.pos[1] = surface.obj[1] = top;
    surface.tex[0] = 1;
    surface.tex[1] = 0;

    m_vertices[1] = surface;

    // BOTTOM RIGHT
    surface.pos[0] = surface.obj[0] = right;
    surface.pos[1] = surface.obj[1] = bottom;
    surface.tex[0] = 1;
    surface.tex[1] = 1;

    m_vertices[2] = surface;

    // BOTTOM LEFT
    surface.pos[0] = surface.obj[0] = left;
    surface.pos[1] = surface.obj[1] = bottom;
    surface.tex[0] = 0;
    surface.tex[1] = 1;

    m_vertices[3] = surface;

    VertexBuffer vbuffer;
    vbuffer.format = kVertexBufferFormat_2f_4ub_2f_2f_28f;
    vbuffer.size = sizeof(Vertex_2f_4ub_2f_2f_28f) * m_vertices.size();
    vbuffer.data = reinterpret_cast<uint8_t*>(m_vertices.data());

    IndexBuffer ibuffer;
    ibuffer.size = sizeof(IndexType) * m_indices.size();
    ibuffer.data = reinterpret_cast<uint8_t*>(m_indices.data());

    if (initial_creation) {
        m_geometryId = m_driver->NextGeometryId();
        m_driver->CreateGeometry(m_geometryId, vbuffer, ibuffer);
    }
    else {
        m_driver->UpdateGeometry(m_geometryId, vbuffer, ibuffer);
    }

    m_needsUpdate = false;
}
