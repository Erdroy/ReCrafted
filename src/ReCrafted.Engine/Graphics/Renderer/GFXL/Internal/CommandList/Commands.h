// GFXL - Graphics Library (c) 2016-2017 Damian 'Erdroy' Korczowski

#pragma once

#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint.h>

#define GFXLCommand(name) GFXL_ALIGN(8)                                 \
struct Command_##name {	                                                \
public:                                                                 \
	Command_##name() : size(sizeof(Command_##name)) { }	                \
public:                                                                 \
	const GFXLCommandHeader::_enum header = GFXLCommandHeader::##name;

namespace GFXL
{
	struct GFXLCommandHeader
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

	GFXLCommand(Empty);
    uint16_t size = 0u; 
	};

	GFXLCommand(ApplyWindow);
    uint16_t size = 0u;
    WindowHandle window;
	};

	GFXLCommand(DestroyWindow);
    uint16_t size = 0u;
    WindowHandle window;
	};

    GFXLCommand(ApplyRenderBuffer);
    uint16_t size = 0u;
    RenderBufferHandle renderBuffer;
    };

    GFXLCommand(ClearRenderBuffer);
    uint16_t size = 0u;
    RenderBufferHandle renderBuffer;
    Color color;
    };

    GFXLCommand(DestroyRenderBuffer);
    uint16_t size = 0u;
    };

    GFXLCommand(CreateShader);
    uint16_t size = 0u;
    ShaderHandle shader;
    char fileName[_MAX_PATH]; // note: we cannot use std::string as this is 'fixed size' type struct!
    };

    GFXLCommand(ApplyShader);
    uint16_t size = 0u;
    ShaderHandle shader;
    uint16_t passId = 0u;
    };

    GFXLCommand(DestroyShader);
    uint16_t size = 0u;
    ShaderHandle shader;
    };


}

#endif // COMMANDS_H
