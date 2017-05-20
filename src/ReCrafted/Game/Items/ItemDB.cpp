// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "ItemDB.h"
#include "../../Core/Logger.h"
#include "../../Graphics/AtlasScaler.h"
#include <FreeImage.h>

ItemDB* ItemDB::m_instance;

void ItemDB::init()
{
	m_instance = new ItemDB;
	m_instance->m_atlas = Texture2D::createTexture();
	m_instance->m_atlas->createMemory(RECRAFTED_BLOCK_ATLAS_SIZE, RECRAFTED_BLOCK_ATLAS_SIZE, RECRAFTED_BLOCK_SAMPLER_FLAGS);
}

void ItemDB::generateAtlases()
{
	// generate mipmaps
	auto main_bits = reinterpret_cast<byte*>(m_instance->m_atlas->getPixels());

	auto elemcount = RECRAFTED_BLOCK_ATLAS_SIZE / RECRAFTED_BLOCK_ATLAS_ITEM_SIZE;

	auto lastSize = RECRAFTED_BLOCK_ATLAS_SIZE;
	auto last_mip = main_bits;

	std::vector<byte*> m_mips = {};

	// generate 4 mips TODO: calculate how much mips we can generate for given atlas/element size
	for(auto i = 0; i < 4; i ++)
	{
		auto mip = AtlasScaler::downscale(last_mip, lastSize, elemcount);
		last_mip = mip;
		lastSize = lastSize / 2;

		m_mips.push_back(mip);

		m_instance->m_atlas->addPixels(lastSize, lastSize, reinterpret_cast<uint*>(mip));
	}

	for(auto mip : m_mips)
		free(mip);

	// upload texture
	m_instance->m_atlas->apply();
	Logger::write("Generated blocks atlas - 1024x1024/32bpp", LogLevel::Info);
}

void ItemDB::registerItem(itemid id, Item item)
{
	if (getItem(id) != nullptr)
		return;

	m_instance->m_items[id] = item;
	m_instance->m_itemCount++;
}

Item_BlockData* ItemDB::createBlockData(itemid id, const char* texture, const char* top_texture, const char* bottom_texture, const char* sideoverlay_texture)
{
	if (texture == nullptr)
		throw;

	if (m_instance->m_items[id].itemType != ItemType::unknown)
		throw;

	if (id > 1024) // temporary
		throw;

	auto data = new Item_BlockData;
	data->flags = ITEM_BLOCKFLAG_NONE;

	// TODO: block side overlays, diff textures

	if (top_texture)
		throw;
		//data->flags = data->flags | ITEM_BLOCKFLAG_HAS_DIFF_TOP;

	if (bottom_texture)
		throw;
		//data->flags = data->flags | ITEM_BLOCKFLAG_HAS_DIFF_BOTTOM;

	if (sideoverlay_texture)
		throw;
		//data->flags = data->flags | ITEM_BLOCKFLAG_HAS_SIDEOVERLAY;

	// calculate texture origin position in pixels
	auto atlas_x = (id - 1) * RECRAFTED_BLOCK_ATLAS_ITEM_SIZE % RECRAFTED_BLOCK_ATLAS_SIZE;
	auto atlas_y = (id - 1) / (RECRAFTED_BLOCK_ATLAS_SIZE / RECRAFTED_BLOCK_ATLAS_ITEM_SIZE) * RECRAFTED_BLOCK_ATLAS_ITEM_SIZE;

	// load texture
	uint* pixels;
	int width;
	int height;
	Texture2D::loadTextureData(texture, &pixels, &width, &height);
	{
		if (pixels == nullptr)
			throw;

		if (width != RECRAFTED_BLOCK_ATLAS_ITEM_SIZE || height != RECRAFTED_BLOCK_ATLAS_ITEM_SIZE)
			throw;


		// copy the texture
		m_instance->m_atlas->setPixels(atlas_x, atlas_y, RECRAFTED_BLOCK_ATLAS_ITEM_SIZE, RECRAFTED_BLOCK_ATLAS_ITEM_SIZE, pixels);
	}

	Texture2D::releaseTextureData(pixels);

	// calculate uvs
	data->texture_u = float(atlas_x) / RECRAFTED_BLOCK_ATLAS_SIZE;
	data->texture_v = float(atlas_y) / RECRAFTED_BLOCK_ATLAS_SIZE;
	data->texture_w = float(RECRAFTED_BLOCK_ATLAS_ITEM_SIZE) / RECRAFTED_BLOCK_ATLAS_SIZE;
	data->texture_h = float(RECRAFTED_BLOCK_ATLAS_ITEM_SIZE) / RECRAFTED_BLOCK_ATLAS_SIZE;

	return data;
}

void ItemDB::shutdown()
{
	if (m_instance == nullptr)
		return;

	delete m_instance;
}
