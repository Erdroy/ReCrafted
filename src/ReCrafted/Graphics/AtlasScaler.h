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

		auto src_elem_size = src_width / elems;

		auto dest_width = src_width / 2;
		auto dest_elem_size = dest_width / elems;
		auto dest_bits = static_cast<byte*>(malloc(dest_width * dest_width * 4));

		auto src_pixels = reinterpret_cast<uint*>(src_bits);
		auto dest_pixels = reinterpret_cast<uint*>(dest_bits);

		for(auto elemY = 0; elemY < elems; elemY ++)
		{
			for (auto elemX = 0; elemX < elems; elemX++)
			{
				for(auto my = 0; my < src_elem_size; my ++)
				{
					for (auto mx = 0; mx < src_elem_size; mx++)
					{
						auto x = elemX * src_elem_size + mx;
						auto y = elemY * src_elem_size + my;

						auto idx = int(y * src_width + x);
						auto pixel_c = src_pixels[idx];
						
						uint pixel_l = 0x0;
						uint pixel_r = 0x0;
						uint pixel_u = 0x0;
						uint pixel_b = 0x0;
						
						auto has_pixel_l = false;
						auto has_pixel_r = false;
						auto has_pixel_u = false;
						auto has_pixel_d = false;

						/*if(idx - 1 > 0)
						{
							pixel_l = src_pixels[idx - 1];
							has_pixel_l = true;
						}

						if(idx + 1 < src_width * src_width)
						{
							pixel_r = src_pixels[idx + 1];
							has_pixel_r = true;
						}

						if (idx - 1 > 0)
						{
							pixel_u = src_pixels[idx - src_width];
							has_pixel_u = true;
						}

						if (idx + 1 < src_width * src_width)
						{
							pixel_b = src_pixels[idx + src_width];
							has_pixel_d = true;
						}

						CHANNELS(pixel_c);
						CHANNELS(pixel_l);
						CHANNELS(pixel_r);
						CHANNELS(pixel_u);
						CHANNELS(pixel_b);*/

						dest_pixels[(y / 2) * dest_width + x] = pixel_c;
					}
				}
			}
		}
		
		return dest_bits;
	}
};

#endif // ATLASSCALER_H
