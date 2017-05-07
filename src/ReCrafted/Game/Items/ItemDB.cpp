// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "ItemDB.h"

ItemDB* ItemDB::m_instance;

void ItemDB::init()
{
	m_instance = new ItemDB;
}

void ItemDB::generateAtlases()
{
	m_instance->m_atlasMem = bgfx::alloc(RECRAFTED_BLOCK_ATLAS_SIZE * RECRAFTED_BLOCK_ATLAS_SIZE * 4);
}

void ItemDB::registerItem(uint id, Item item)
{
	if (getItem(id) != nullptr)
		return;

	m_instance->m_items[id] = item;
	m_instance->m_itemCount++;
}

Item_BlockData* ItemDB::registerBlock(int id, const char* texture, const char* top_texture, const char* bottom_texture, const char* sideoverlay_texture)
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

	// copy the texture

	return data;
}

void ItemDB::shutdown()
{
	if (m_instance == nullptr)
		return;

	delete m_instance;
}
