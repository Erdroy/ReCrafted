// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef ATLASSCALER_H
#define ATLASSCALER_H

// includes
#include "../Utils/Types.h"

class AtlasScaler
{
public:
	static byte* downscale(byte* src_bits, uint src_width, uint elems)
	{
#define CHANNELS(pixel) \
		byte pixel##_r = (pixel & 0xFF000000) >> 24; \
		byte pixel##_g = (pixel & 0x00FF0000) >> 16; \
		byte pixel##_b = (pixel & 0x0000FF00) >> 8; \
		byte pixel##_a = (pixel & 0x000000FF); \


		auto dest_width = src_width / 2;
		auto dest_bits = static_cast<byte*>(malloc(dest_width * dest_width * 4));

		auto src_elem_size = src_width / elems;
		auto dest_elem_size = dest_width / elems;

		auto src_pixels = reinterpret_cast<uint*>(src_bits);
		auto dest_pixels = reinterpret_cast<uint*>(dest_bits);

		for(auto elemY = 0u; elemY < elems; elemY ++)
		{
			for (auto elemX = 0u; elemX < elems; elemX ++)
			{
				auto ox = elemX * src_elem_size;
				auto oy = elemY * src_elem_size;
				
				for(auto my = 0u; my < src_elem_size; my += 2)
				{
					for (auto mx = 0u; mx < src_elem_size; mx += 2)
					{
						auto x = ox + mx;
						auto y = oy + my;

						// sample neigh pixels
						uint pixel_c = 0x0;
						uint pixel_r = 0x0;
						uint pixel_d1 = 0x0;
						uint pixel_d2 = 0x0;

						pixel_c = src_pixels[y * src_width + x];
						pixel_r = src_pixels[y * src_width + (x + 1)];
						pixel_d1 = src_pixels[(y + 1) * src_width + x];
						pixel_d2 = src_pixels[(y + 1) * src_width + (x + 1)];

						auto dpx_r = 0x0u;
						auto dpx_g = 0x0u;
						auto dpx_b = 0x0u;
						auto dpx_a = 0x0u;

						CHANNELS(pixel_c)
						{
							dpx_r += pixel_c_r;
							dpx_g += pixel_c_g;
							dpx_b += pixel_c_b;
							dpx_a += pixel_c_a;
						}

						CHANNELS(pixel_r)
						{
							dpx_r += pixel_r_r;
							dpx_g += pixel_r_g;
							dpx_b += pixel_r_b;
							dpx_a += pixel_r_a;
						}

						CHANNELS(pixel_d1)
						{
							dpx_r += pixel_d1_r;
							dpx_g += pixel_d1_g;
							dpx_b += pixel_d1_b;
							dpx_a += pixel_d1_a;
						}

						CHANNELS(pixel_d2)
						{
							dpx_r += pixel_d2_r;
							dpx_g += pixel_d2_g;
							dpx_b += pixel_d2_b;
							dpx_a += pixel_d2_a;
						}

						dpx_r /= 4;
						dpx_g /= 4;
						dpx_b /= 4;
						dpx_a /= 4;

						auto dox = elemX * dest_elem_size;
						auto doy = elemY * dest_elem_size;

						auto dx = dox + mx / 2;
						auto dy = doy + my / 2;

						dest_pixels[dy * dest_width + dx] = 0u
						| byte(dpx_a)
						| byte(dpx_b) << 8 
						| byte(dpx_g) << 16 
						| byte(dpx_r) << 24;
					}
				}
			}
		}
		
		return dest_bits;
	}
};

#endif // ATLASSCALER_H
