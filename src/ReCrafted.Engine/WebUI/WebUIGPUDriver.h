// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef WEBUIGPUDRIVER_H
#define WEBUIGPUDRIVER_H

#include "ReCrafted.h"
#include "Core/Math/Math.h"
#include "Graphics/Shader.h"

#include <Ultralight/platform/GPUDriver.h>

class WebUIGPUDriver : public ultralight::GPUDriver
{
    struct GPUUniforms {
        Vector4 State{};
        Matrix Transform{};
        Vector4 Scalar4[2];
        Vector4 Vector[8];
        uint32_t ClipSize{};
        Matrix Clip[8];
    };

private:
    Shader* m_shader = nullptr;
    std::vector<ultralight::Command> m_commandList;

    GPUUniforms m_uniforms = {};

private:
    void UpdateUniforms(const ultralight::GPUState& state);

public:
    WebUIGPUDriver();
    ~WebUIGPUDriver();

public:
    void BeginSynchronize() override;
    void EndSynchronize() override;
    
    uint32_t NextTextureId() override;
    void CreateTexture(uint32_t texture_id, ultralight::Ref<ultralight::Bitmap> bitmap) override;
    void UpdateTexture(uint32_t texture_id, ultralight::Ref<ultralight::Bitmap> bitmap) override;
    void BindTexture(uint8_t texture_unit, uint32_t texture_id) override;
    void DestroyTexture(uint32_t texture_id) override;

    uint32_t NextRenderBufferId() override;
    void CreateRenderBuffer(uint32_t render_buffer_id, const ultralight::RenderBuffer& buffer) override;
    void BindRenderBuffer(uint32_t render_buffer_id) override;
    void SetRenderBufferViewport(uint32_t render_buffer_id, uint32_t width, uint32_t height) override;
    void ClearRenderBuffer(uint32_t render_buffer_id) override;
    void DestroyRenderBuffer(uint32_t render_buffer_id) override;

    uint32_t NextGeometryId() override;
    void CreateGeometry(uint32_t geometry_id, const ultralight::VertexBuffer& vertices, const ultralight::IndexBuffer& indices) override;
    void UpdateGeometry(uint32_t geometry_id, const ultralight::VertexBuffer& vertices, const ultralight::IndexBuffer& indices) override;
    void DrawGeometry(uint32_t geometry_id, uint32_t indices_count, uint32_t indices_offset, const ultralight::GPUState& state) override;
    void DestroyGeometry(uint32_t geometry_id) override;

    void UpdateCommandList(const ultralight::CommandList& list) override;
    bool HasCommandsPending() override;
    void DrawCommandList() override;
};

#endif // WEBUIGPUDRIVER_H
