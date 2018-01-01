// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef BGFXPREREQUISITES_H
#define BGFXPREREQUISITES_H

#define BGFX_CONFIG_MAX_VERTEX_BUFFERS 65535
#define BGFX_CONFIG_MAX_INDEX_BUFFERS  65535

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <math.h>

namespace bx
{
	inline float flog2(float _a)
	{
		return logf(_a) * 1.442695041f;
	}

	inline uint32_t uint32_min(uint32_t _a, uint32_t _b)
	{
		return _a > _b ? _b : _a;
	}

	inline uint32_t uint32_min(uint32_t _a, uint32_t _b, uint32_t _c)
	{
		return uint32_min(_a, uint32_min(_b, _c));
	}

	inline uint32_t uint32_max(uint32_t _a, uint32_t _b)
	{
		return _a > _b ? _a : _b;
	}
}

namespace bgfx
{
	inline uint8_t calcNumMips(bool _hasMips, uint16_t _width, uint16_t _height, uint16_t _depth = 1)
	{
		if (_hasMips)
		{
			const uint32_t max = bx::uint32_max(bx::uint32_max(_width, _height), _depth);
			const uint32_t num = uint32_t(bx::flog2(float(max)));

			return uint8_t(num);
		}

		return 0;
	}
}

#endif // BGFXPREREQUISITES_H