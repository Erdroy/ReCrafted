// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint.h>

#define Command(name) RENDERER_ALIGN(8)                                 \
struct Command_##name {	                                                \
public:                                                                 \
	Command_##name() : size(sizeof(Command_##name)) { }	                \
public:                                                                 \
	const CommandHeader::_enum header = CommandHeader::##name;

namespace Renderer
{
	struct CommandHeader
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

	Command(Empty);
    uint16_t size = 0u; 
	};

	Command(ApplyWindow);
    uint16_t size = 0u;
    WindowHandle window;
	};

	Command(DestroyWindow);
    uint16_t size = 0u;
    WindowHandle window;
	};

    Command(ApplyRenderBuffer);
    uint16_t size = 0u;
    RenderBufferHandle renderBuffer;
    };

    Command(ClearRenderBuffer);
    uint16_t size = 0u;
    RenderBufferHandle renderBuffer;
    Color color;
    };

    Command(DestroyRenderBuffer);
    uint16_t size = 0u;
    };

    Command(CreateShader);
    uint16_t size = 0u;
    ShaderHandle shader;
    char fileName[_MAX_PATH]; // note: we cannot use std::string as this is 'fixed size' type struct!
    };

    Command(ApplyShader);
    uint16_t size = 0u;
    ShaderHandle shader;
    uint16_t passId = 0u;
    };

    Command(DestroyShader);
    uint16_t size = 0u;
    ShaderHandle shader;
    };


}

#endif // COMMANDS_H
