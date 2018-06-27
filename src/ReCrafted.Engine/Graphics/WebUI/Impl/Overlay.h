#pragma once
#include <Ultralight/View.h>
#include <Ultralight/Renderer.h>
#include <Ultralight/platform/GPUDriver.h>
#include <vector>

#include "Graphics/WebUI/WebUIOverlay.h"

using namespace ultralight;

enum MouseButton {
    kMouseButton_Left,
    kMouseButton_Middle,
    kMouseButton_Right,
};

/**
* D3D Screen Overlay that wraps a View, useful for UI purposes.
*/
class Overlay : public WebUIOverlay, public LoadListener, public ViewListener {
private:
    bool m_fullscreen;
    bool m_needsUpdate = true;
    uint32_t m_geometryId = 0;
    GPUState m_gpuState;
    uint32_t m_texture = 0u;
    uint32_t m_renderBuffer = 0u;
    int m_width;
    int m_height;

    ultralight::Ref<View> m_view;
    GPUDriver* m_driver;
    std::vector<Vertex_2f_4ub_2f_2f_28f> m_vertices;
    std::vector<IndexType> m_indices;

public:
    Overlay(bool fullscreen, ultralight::Renderer* renderer, GPUDriver* driver, int width, int height);
    virtual ~Overlay();

public:
    void Draw() override;
    void Resize(uint32_t width, uint32_t height) override;

    void OnFinishLoading(ultralight::View* caller) override;
    void OnBeginLoading(ultralight::View* caller) override;
    void OnUpdateHistory(ultralight::View* caller) override;
    void OnDOMReady(ultralight::View* caller) override;
    void OnAddConsoleMessage(ultralight::View* caller, MessageSource source, MessageLevel level, const String& message,
                             uint32_t line_number, uint32_t column_number, const String& source_id) override;

    void KeyDown(WPARAM wparam, LPARAM lparam, bool is_system_key) const;
    void KeyUp(WPARAM wparam, LPARAM lparam, bool is_system_key) const;
    void CharPress(WPARAM wparam, LPARAM lparam, bool is_system_key) const;
    void MouseMove(int x, int y) const;
    void MouseButtonDown(MouseButton button, int x, int y) const;
    void MouseButtonUp(MouseButton button, int x, int y) const;
    void MouseWheel(int delta) const;

    ultralight::Ref<View> view() const
    {
        return m_view;
    }

    int width() const
    {
        return m_width;
    }
    int height() const
    {
        return m_height;
    }

protected:
    void UpdateGeometry();
};