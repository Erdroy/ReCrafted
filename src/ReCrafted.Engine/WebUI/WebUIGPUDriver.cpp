// ReCrafted (c) 2016-2018 Always Too Late

#include "WebUIGPUDriver.h"

void WebUIGPUDriver::BeginSynchronize()
{
}

void WebUIGPUDriver::EndSynchronize()
{
}

uint32_t WebUIGPUDriver::NextTextureId()
{
    return 0u;
}

void WebUIGPUDriver::CreateTexture(uint32_t texture_id, ultralight::Ref<ultralight::Bitmap> bitmap)
{
}

void WebUIGPUDriver::UpdateTexture(uint32_t texture_id, ultralight::Ref<ultralight::Bitmap> bitmap)
{
}

void WebUIGPUDriver::BindTexture(uint8_t texture_unit, uint32_t texture_id)
{
}

void WebUIGPUDriver::DestroyTexture(uint32_t texture_id)
{
}

uint32_t WebUIGPUDriver::NextRenderBufferId()
{
    return 0u;
}

void WebUIGPUDriver::CreateRenderBuffer(uint32_t render_buffer_id, const ultralight::RenderBuffer& buffer)
{
}

void WebUIGPUDriver::BindRenderBuffer(uint32_t render_buffer_id)
{
}

void WebUIGPUDriver::SetRenderBufferViewport(uint32_t render_buffer_id, uint32_t width, uint32_t height)
{
}

void WebUIGPUDriver::ClearRenderBuffer(uint32_t render_buffer_id)
{
}

void WebUIGPUDriver::DestroyRenderBuffer(uint32_t render_buffer_id)
{
}

uint32_t WebUIGPUDriver::NextGeometryId()
{
    return 0u;
}

void WebUIGPUDriver::CreateGeometry(uint32_t geometry_id, const ultralight::VertexBuffer& vertices,
    const ultralight::IndexBuffer& indices)
{
}

void WebUIGPUDriver::UpdateGeometry(uint32_t geometry_id, const ultralight::VertexBuffer& vertices,
    const ultralight::IndexBuffer& indices)
{
}

void WebUIGPUDriver::DrawGeometry(uint32_t geometry_id, uint32_t indices_count, uint32_t indices_offset,
    const ultralight::GPUState& state)
{
}

void WebUIGPUDriver::DestroyGeometry(uint32_t geometry_id)
{
}

void WebUIGPUDriver::UpdateCommandList(const ultralight::CommandList& list)
{
}

bool WebUIGPUDriver::HasCommandsPending()
{
    return false;
}

void WebUIGPUDriver::DrawCommandList()
{
}
