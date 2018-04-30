// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef COMMANDS_H
#define COMMANDS_H

#include <cstdint>

#define Command(name, body) RENDERER_ALIGN(8) struct Command_##name {   \
    public:                                                             \
	Command_##name() : size(sizeof(Command_##name)) { }	                \
public:                                                                 \
	const CommandHeader::_enum header = CommandHeader::##name;          \
    uint16_t size = 0u;                                                 \
    body                                                                \
};

namespace Renderer
{
	struct CommandHeader
	{
		enum _enum : uint16_t
		{
			Empty,

            Draw,
            DrawIndexed,

			ApplyWindow,
			DestroyWindow,

            CreateRenderBuffer,
            ApplyRenderBuffer,
            ClearRenderBuffer,
            DestroyRenderBuffer,

            CreateVertexBuffer,
            ApplyVertexBuffer,
            UpdateVertexBuffer,
            DestroyVertexBuffer,

            CreateIndexBuffer,
            ApplyIndexBuffer,
            UpdateIndexBuffer,
            DestroyIndexBuffer,

            CreateTexture2D,
            ApplyTexture2D,
            UpdateTexture2D,
            DestroyTexture2D,

            CreateShader,
            SetShaderValue,
            ApplyShader,
		    DestroyShader,
		};
	};

    Command(Empty, );

    Command(Draw,
        uint vertexCount = 0u;
    );
    Command(DrawIndexed,
        uint indexCount = 0u;
    );

    Command(ApplyWindow,
        WindowHandle window;
    );

	Command(DestroyWindow,
        WindowHandle window;
    );

    Command(CreateRenderBuffer,
        RenderBufferHandle handle;
        uint16_t width;
        uint16_t height;
        uint8_t texturesCount;
        Texture2DHandle renderTargets[RENDERER_MAX_RENDER_BUFFER_TARGETS];
        Texture2DHandle depthTarget;
        bool createDepthStencil;
    ); 
    
    Command(ApplyRenderBuffer,
        RenderBufferHandle handle;
    );

    Command(ClearRenderBuffer,
        RenderBufferHandle handle;
        Color color;
    );

    Command(DestroyRenderBuffer,
        RenderBufferHandle handle;
    );

    Command(CreateShader,
        ShaderHandle shader;
        char fileName[_MAX_PATH]; // note: we cannot use std::string as this is 'fixed size' type struct!
    );

    Command(ApplyShader,
        ShaderHandle shader;
        uint16_t passId = 0u;
    );

    Command(SetShaderValue, // CUSTOM!
        ShaderHandle shader;
        uint8_t bufferId;
        uint8_t fieldId;
        uint8_t dataSize;
    );

    Command(DestroyShader,
        ShaderHandle shader;
    );

    Command(CreateVertexBuffer,
        VertexBufferHandle handle;
        RendererMemory memory;
        uint vertexCount;
        uint8_t vertexSize;
        bool dynamic;
    );

    Command(ApplyVertexBuffer,
        VertexBufferHandle handle;
    );

    Command(DestroyVertexBuffer,
        VertexBufferHandle handle;
    );

    Command(CreateIndexBuffer,
        IndexBufferHandle handle;
        RendererMemory memory;
        uint indexCount;
        uint8_t indexSize;
        bool dynamic;
    );

    Command(ApplyIndexBuffer,
        IndexBufferHandle handle;
    );

    Command(DestroyIndexBuffer,
        IndexBufferHandle handle;
    );

    Command(CreateTexture2D,
        Texture2DHandle handle;
        uint16_t width;
        uint16_t height;
        uint8_t mipLevels;
        TextureFormat::_enum textureFormat;
        RendererMemory memory;
        size_t dataSize;
        bool renderTarget;
    );

    Command(ApplyTexture2D,
        Texture2DHandle handle;
        uint8_t slot;
    );

    Command(DestroyTexture2D,
        Texture2DHandle handle;
    );
}

#endif // COMMANDS_H
