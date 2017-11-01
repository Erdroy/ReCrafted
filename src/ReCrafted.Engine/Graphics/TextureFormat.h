// ReCrafted © 2016-2017 Always Too Late

#pragma once

#ifndef TEXTUREFORMAT_H
#define TEXTUREFORMAT_H

/// <summary>
/// Texture format enum.
/// </summary>
struct TextureFormat
{
	enum Enum
	{
		BC1,          //!< DXT1
		BC2,          //!< DXT3
		BC3,          //!< DXT5
		BC4,          //!< LATC1/ATI1
		BC5,          //!< LATC2/ATI2
		BC6H,         //!< BC6H
		BC7,          //!< BC7
		ETC1,         //!< ETC1 RGB8
		ETC2,         //!< ETC2 RGB8
		ETC2A,        //!< ETC2 RGBA8
		ETC2A1,       //!< ETC2 RGB8A1
		PTC12,        //!< PVRTC1 RGB 2BPP
		PTC14,        //!< PVRTC1 RGB 4BPP
		PTC12A,       //!< PVRTC1 RGBA 2BPP
		PTC14A,       //!< PVRTC1 RGBA 4BPP
		PTC22,        //!< PVRTC2 RGBA 2BPP
		PTC24,        //!< PVRTC2 RGBA 4BPP

		Unknown,      // Compressed formats above.

		R1,
		A8,
		R8,
		R8I,
		R8U,
		R8S,
		R16,
		R16I,
		R16U,
		R16F,
		R16S,
		R32I,
		R32U,
		R32F,
		RG8,
		RG8I,
		RG8U,
		RG8S,
		RG16,
		RG16I,
		RG16U,
		RG16F,
		RG16S,
		RG32I,
		RG32U,
		RG32F,
		RGB8,
		RGB8I,
		RGB8U,
		RGB8S,
		RGB9E5F,
		BGRA8,
		RGBA8,
		RGBA8I,
		RGBA8U,
		RGBA8S,
		RGBA16,
		RGBA16I,
		RGBA16U,
		RGBA16F,
		RGBA16S,
		RGBA32I,
		RGBA32U,
		RGBA32F,
		R5G6B5,
		RGBA4,
		RGB5A1,
		RGB10A2,
		R11G11B10F,

		UnknownDepth, // Depth formats below.

		D16,
		D24,
		D24S8,
		D32,
		D16F,
		D24F,
		D32F,
		D0S8,

		Count
	};
};

#endif // TEXTUREFORMAT_H
