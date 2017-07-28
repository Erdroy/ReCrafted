// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Texture2D.h"
#include "upng/upng.h"

static void releaseBits(void* _ptr, void* _userData)
{
	upng::upng_free(static_cast<upng_t*>(_userData));
}

static void releaseBitsRaw(void* _ptr, void* _userData)
{
	free(_userData);
}

void Texture2D::loadFile(const char* filename, uint flags)
{
	m_bitmap = upng::upng_new_from_file(filename);

	if (m_bitmap == nullptr)
		throw;

	upng::upng_decode(m_bitmap);

	m_bits = const_cast<byte*>(upng::upng_get_buffer(m_bitmap));
	m_width = upng::upng_get_width(m_bitmap);
	m_height = upng::upng_get_height(m_bitmap);
	m_flags = flags;
	
	// done!

	// FreeImage bitmap will be unloaded with in `releaseBits`
}

void Texture2D::loadMemory(void* data, int width, int height, uint flags)
{
	m_width = width;
	m_height = height;
	m_flags = flags;

	m_bits = static_cast<byte*>(malloc(width * height * 4));

	memset(m_bits, 0, width * height * 4);

	if(data)
		memcpy(m_bits, data, width * height * 4);

	// done

	// bits will be unloaded with in `releaseBitsRaw`
}

void Texture2D::createMemory(int width, int height, uint flags)
{
	loadMemory(nullptr, width, height, flags);
}

void Texture2D::addMip(int width, int height, uint* pixels)
{
	if (m_bitmap)
		throw;

	auto size = width * height * 4;
	auto baseSize = m_width * m_height * 4;

	if (m_mips > 0)
	{
		auto lwidth = m_width;
		for (auto i = 0u; i < m_mips; i++)
		{
			auto w = lwidth / 2;
			baseSize += w * w * 4;
			lwidth = w;
		}
	}

	// alloc more memory
	auto newSize = baseSize + size;
	auto bits = realloc(m_bits, newSize);

	if (bits)
	{
		m_bits = static_cast<byte*>(bits);

		// copy
		auto dest = m_bits + baseSize;
		memcpy(dest, pixels, size);

		m_mips++;
	}
}

uint Texture2D::getPixel(int x, int y)
{
	if (m_bits == nullptr)
		return 0x0u;

	auto idx = y * m_width + x;

	auto r = m_bits[idx + 0];
	auto g = m_bits[idx + 1];
	auto b = m_bits[idx + 2];
	auto a = m_bits[idx + 3];

	return a | b << 8 | g << 16 | r << 24;
}

uint* Texture2D::getPixels() const
{
	return reinterpret_cast<uint*>(m_bits);
}

void Texture2D::setPixel(int x, int y, uint pixel)
{
	if (m_bits == nullptr)
		return;

	auto idx = y * m_width + x;
	auto pixels = reinterpret_cast<uint*>(m_bits);
	pixels[idx] = pixel;
}

void Texture2D::setPixel(int x, int y, byte r, byte g, byte b, byte a)
{
	if (m_bits == nullptr)
		return;

	auto idx = y * m_width + x;
	auto pixels = reinterpret_cast<uint*>(m_bits);
	pixels[idx] = a | b << 8 | g << 16 | r << 24;
}

void Texture2D::setPixels(int x, int y, int width, int height, uint* pixels)
{
	auto mpixels = reinterpret_cast<uint*>(m_bits);
	for(auto my = y; my < y + height; my++)
	{
		for (auto mx = x; mx < x + width; mx++)
		{
			mpixels[my * m_width + mx] = *pixels++;
		}
	}
}

uint Texture2D::getHandle()
{
	return m_textureHandle.idx;
}

uint Texture2D::getWidth() const
{
	return m_width;
}

uint Texture2D::getHeight() const
{
	return m_height;
}

void Texture2D::apply()
{
	auto size = m_width * m_height * 4;

	if (m_mips > 0)
	{
		auto lwidth = m_width;
		for (auto i = 0u; i < m_mips; i++)
		{
			auto w = lwidth / 2;
			size += w * w * 4;
			lwidth = w;
		}
	}

	const bgfx::Memory* mem;
	if (m_bitmap)
		mem = bgfx::makeRef(m_bits, size, releaseBits, static_cast<void*>(m_bitmap));
	else
		mem = bgfx::makeRef(m_bits, size, releaseBitsRaw, static_cast<void*>(m_bits));

	memcpy_s(mem->data, mem->size, m_bits, size);
	
	auto mipCount = uint8_t(m_mips + 1);
	m_textureHandle = bgfx::createTexture2D(uint16_t(m_width), uint16_t(m_height), mipCount, 1, bgfx::TextureFormat::RGBA8, m_flags, mem);

	m_bits = nullptr;
}

void Texture2D::dispose()
{
	bgfx::destroyTexture(m_textureHandle);

	if(m_bits)
	{
		if(m_bitmap)
		{
			upng::upng_free(m_bitmap);
			m_bits = nullptr;
			m_bitmap = nullptr;
		}
		else
		{
			free(m_bits);
			m_bits = nullptr;
		}
	}
}

void Texture2D::loadTextureData(const char* filename, uint** pixels, int* width, int* height)
{
	auto bitmap = upng::upng_new_from_file(filename);
	upng::upng_decode(bitmap);

	auto bits = const_cast<byte*>(upng::upng_get_buffer(bitmap));
	*width = upng::upng_get_width(bitmap);
	*height = upng::upng_get_height(bitmap);

	*pixels = new uint[*width * *height];

	memcpy(*pixels, bits, *width * *height * 4);
}

void Texture2D::releaseTextureData(uint* pixels)
{
	delete pixels;
}

void Texture2D::saveBitmap(const char* filename, uint width, uint height, byte* bitsRaw)
{
	FILE *f;
	unsigned char *img = NULL;
	int filesize = 54 + 3 * width*height;  //w is your image width, h is image height, both int
	if (img)
		free(img);
	img = (unsigned char *)malloc(3 * width*height);
	memset(img, 0, sizeof(img));

	auto idx = 0;
	for(auto i = 0u; i < width * height * 4; i += 4)
	{
		img[idx + 0] = bitsRaw[i + 0];
		img[idx + 1] = bitsRaw[i + 1];
		img[idx + 2] = bitsRaw[i + 2];

		idx += 3;
	}

	unsigned char bmpfileheader[14] = { 'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0 };
	unsigned char bmpinfoheader[40] = { 40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0 };
	unsigned char bmppad[3] = { 0,0,0 };

	bmpfileheader[2] = static_cast<unsigned char>(filesize);
	bmpfileheader[3] = static_cast<unsigned char>(filesize >> 8);
	bmpfileheader[4] = static_cast<unsigned char>(filesize >> 16);
	bmpfileheader[5] = static_cast<unsigned char>(filesize >> 24);

	bmpinfoheader[4] = static_cast<unsigned char>(width);
	bmpinfoheader[5] = static_cast<unsigned char>(width >> 8);
	bmpinfoheader[6] = static_cast<unsigned char>(width >> 16);
	bmpinfoheader[7] = static_cast<unsigned char>(width >> 24);
	bmpinfoheader[8] = static_cast<unsigned char>(height);
	bmpinfoheader[9] = static_cast<unsigned char>(height >> 8);
	bmpinfoheader[10] = static_cast<unsigned char>(height >> 16);
	bmpinfoheader[11] = static_cast<unsigned char>(height >> 24);

	f = fopen(filename, "wb");
	fwrite(bmpfileheader, 1, 14, f);
	fwrite(bmpinfoheader, 1, 40, f);
	for (auto i = 0u; i<height; i++)
	{
		fwrite(img + (width*(height - i - 1) * 3), 3, width, f);
		fwrite(bmppad, 1, (4 - (width * 3) % 4) % 4, f);
	}
	fclose(f);
}

Ptr<Texture2D> Texture2D::createTexture()
{
	Ptr<Texture2D> texture(new Texture2D);
	return texture;
}