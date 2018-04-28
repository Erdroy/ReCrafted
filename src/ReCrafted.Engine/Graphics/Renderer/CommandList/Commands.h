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

            ApplyRenderBuffer,
            ClearRenderBuffer,
            DestroyRenderBuffer,

            CreateVertexBuffer,
            ApplyVertexBuffer,
            UpdateVertexBuffer,
            DestroyVertexBuffer,

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

    Command(ApplyRenderBuffer,
        RenderBufferHandle renderBuffer;
    );

    Command(ClearRenderBuffer,
        RenderBufferHandle renderBuffer;
        Color color;
    );

    Command(DestroyRenderBuffer,
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
        RendererMemory memory;
        VertexBufferHandle handle;
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
}

#endif // COMMANDS_H
