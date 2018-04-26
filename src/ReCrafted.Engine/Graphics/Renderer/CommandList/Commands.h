// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint.h>

#define RendererCommand(name) RENDERER_ALIGN(8)                                 \
struct Command_##name {	                                                \
public:                                                                 \
	Command_##name() : size(sizeof(Command_##name)) { }	                \
public:                                                                 \
	const RendererCommandHeader::_enum header = RendererCommandHeader::##name;

namespace Renderer
{
	struct RendererCommandHeader
	{
		enum _enum : uint16_t
		{
			Empty,

			ApplyWindow,
			DestroyWindow,

            ApplyRenderBuffer,
            ClearRenderBuffer,
            DestroyRenderBuffer,

            CreateShader,
            ApplyShader,
		    DestroyShader,
		};
	};

	RendererCommand(Empty);
    uint16_t size = 0u; 
	};

	RendererCommand(ApplyWindow);
    uint16_t size = 0u;
    WindowHandle window;
	};

	RendererCommand(DestroyWindow);
    uint16_t size = 0u;
    WindowHandle window;
	};

    RendererCommand(ApplyRenderBuffer);
    uint16_t size = 0u;
    RenderBufferHandle renderBuffer;
    };

    RendererCommand(ClearRenderBuffer);
    uint16_t size = 0u;
    RenderBufferHandle renderBuffer;
    Color color;
    };

    RendererCommand(DestroyRenderBuffer);
    uint16_t size = 0u;
    };

    RendererCommand(CreateShader);
    uint16_t size = 0u;
    ShaderHandle shader;
    char fileName[_MAX_PATH]; // note: we cannot use std::string as this is 'fixed size' type struct!
    };

    RendererCommand(ApplyShader);
    uint16_t size = 0u;
    ShaderHandle shader;
    uint16_t passId = 0u;
    };

    RendererCommand(DestroyShader);
    uint16_t size = 0u;
    ShaderHandle shader;
    };


}

#endif // COMMANDS_H
