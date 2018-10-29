// ReCrafted (c) 2016-2018 Always Too Late

#include "WebUIGPUDriver.h"
#include "Content/ContentManager.h"
#include "Graphics/Graphics.h"

struct GeometryHandle : ObjectHandle { };
struct GemetryDescription
{
    Renderer::VertexBufferHandle vertexBuffer;
    Renderer::IndexBufferHandle indexBuffer;
};

typedef HandlePool<GeometryHandle, GemetryDescription> GeometryPool;

WebUIGPUDriver::WebUIGPUDriver()
{
    GeometryPool::Instance()->Initialize(512, true);

    m_shader = ContentManager::LoadAsset<Shader>("Shaders/WebUI");
}

WebUIGPUDriver::~WebUIGPUDriver()
{
    m_shader->Unload();
    m_shader = nullptr;
}

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
    //return Renderer::CreateRenderBuffer();
    return 0u;
}

void WebUIGPUDriver::CreateRenderBuffer(const uint32_t render_buffer_id, const ultralight::RenderBuffer& buffer)
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
    return GeometryPool::AllocateHandle().idx;
}

void WebUIGPUDriver::CreateGeometry(const uint32_t geometry_id, const ultralight::VertexBuffer& vertices,
    const ultralight::IndexBuffer& indices)
{
    GeometryHandle handle;
    handle.idx = geometry_id;
    rvar desc = GeometryPool::GetHandleDescription(handle);

    cvar vertexSize = sizeof(ultralight::Vertex_2f_4ub_2f_2f_28f);
    cvar vertexCount = vertices.size / vertexSize;

    cvar indexCount = indices.size / sizeof(*indices.data);

    desc.vertexBuffer = Renderer::CreateVertexBuffer(vertexCount, vertexSize, true);
    Renderer::UpdateVertexBuffer(desc.vertexBuffer, vertices.data, vertexSize, 0u);

    desc.indexBuffer = Renderer::CreateIndexBuffer(indexCount, false, true);
    Renderer::UpdateIndexBuffer(desc.indexBuffer, indices.data, indices.size, 0);
}

void WebUIGPUDriver::UpdateGeometry(const uint32_t geometry_id, const ultralight::VertexBuffer& vertices,
    const ultralight::IndexBuffer& indices)
{
    GeometryHandle handle;
    handle.idx = geometry_id;
    rvar desc = GeometryPool::GetHandleDescription(handle);

    cvar vertexSize = sizeof(ultralight::Vertex_2f_4ub_2f_2f_28f);
    cvar vertexCount = vertices.size / vertexSize;

    Renderer::UpdateVertexBuffer(desc.vertexBuffer, vertices.data, vertexSize, 0u);
    Renderer::UpdateIndexBuffer(desc.indexBuffer, indices.data, indices.size, 0);
}

void WebUIGPUDriver::DrawGeometry(const uint32_t geometry_id, const uint32_t indices_count, const uint32_t indices_offset,
    const ultralight::GPUState& state)
{
    GeometryHandle handle;
    handle.idx = geometry_id;
    rvar desc = GeometryPool::GetHandleDescription(handle);

    /*m_shader->SetTexture(0, )
    m_shader->SetValue()*/

    // TODO: Apply GPUState

    // Apply shader
    Renderer::ApplyShader(m_shader->GetHandle(), 0);

    // Draw
    Renderer::ApplyVertexBuffer(desc.vertexBuffer);
    Renderer::ApplyIndexBuffer(desc.indexBuffer);
    Renderer::DrawIndexed(indices_count);
}

void WebUIGPUDriver::DestroyGeometry(const uint32_t geometry_id)
{
    GeometryHandle handle;
    handle.idx = geometry_id;
    rvar desc = GeometryPool::GetHandleDescription(handle);

    Renderer::DestroyVertexBuffer(desc.vertexBuffer);
    Renderer::DestroyIndexBuffer(desc.indexBuffer);

    GeometryPool::FreeHandle(handle);
}

void WebUIGPUDriver::UpdateCommandList(const ultralight::CommandList& list)
{
    if (list.size) {
        m_commandList.resize(list.size);
        memcpy(&m_commandList[0], list.commands, sizeof(ultralight::Command) * list.size);
    }
}

bool WebUIGPUDriver::HasCommandsPending()
{
    return false;
}

void WebUIGPUDriver::DrawCommandList()
{
    if (m_commandList.empty())
        return;

    for (auto& cmd : m_commandList) {
        if (cmd.command_type == ultralight::kCommandType_DrawGeometry)
            DrawGeometry(cmd.geometry_id, cmd.indices_count, cmd.indices_offset, cmd.gpu_state);
        else if (cmd.command_type == ultralight::kCommandType_ClearRenderBuffer)
            ClearRenderBuffer(cmd.gpu_state.render_buffer_id);
    }

    m_commandList.clear();
}
