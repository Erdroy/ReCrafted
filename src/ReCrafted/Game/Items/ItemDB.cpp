// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "ItemDB.h"
#include "../../Core/Logger.h"
#include "../../Graphics/AtlasScaler.h"

ItemDB* ItemDB::m_instance;

void ItemDB::init()
{
	m_instance = new ItemDB;
	m_instance->m_atlas = Texture2D::createTexture();
	m_instance->m_atlas->createMemory(RECRAFTED_BLOCK_ATLAS_SIZE, RECRAFTED_BLOCK_ATLAS_SIZE, RECRAFTED_BLOCK_SAMPLER_FLAGS);
}

void ItemDB::generateAtlases()
{
	// TODO: generate mipmaps

	auto main_bits = reinterpret_cast<byte*>(m_instance->m_atlas->getPixels());

	auto mip_count = bgfx::calcNumMips(true, RECRAFTED_BLOCK_ATLAS_SIZE, RECRAFTED_BLOCK_ATLAS_SIZE);

	/*auto mip1size = RECRAFTED_BLOCK_ATLAS_SIZE / 2;
	auto mip2size = mip1size / 2;
	auto mip3size = mip2size / 2;

	auto elemcount = RECRAFTED_BLOCK_ATLAS_SIZE / RECRAFTED_BLOCK_ATLAS_ITEM_SIZE;

	auto mip_1 = AtlasScaler::downscale(main_bits, RECRAFTED_BLOCK_ATLAS_SIZE, elemcount);
	//auto mip_2 = AtlasScaler::downscale(mip_1, mip1size, elemcount);
	//auto mip_3 = AtlasScaler::downscale(mip_2, mip2size, elemcount);

	// add all mips
	m_instance->m_atlas->addPixels(mip1size, mip1size, reinterpret_cast<uint*>(mip_1));
	//m_instance->m_atlas->addPixels(mip2size, mip2size, reinterpret_cast<uint*>(mip_2));
	//m_instance->m_atlas->addPixels(mip3size, mip3size, reinterpret_cast<uint*>(mip_3));*/

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
