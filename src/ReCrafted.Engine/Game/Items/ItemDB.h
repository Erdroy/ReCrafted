// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef ITEMDB_H
#define ITEMDB_H

// includes
#include "Core/Defines.h"
#include "recraftedConfig.h"
#include "Voxels/VoxelChunk.h"
#include "Item.h"
#include "ItemID.h"

/// <summary>
/// Item database class.
/// Handles all in-game items.
/// </summary>
class ItemDB
{
	friend class Rendering;

private:
	static ItemDB* m_instance;

private:
	Item m_items[RECRAFTED_MAX_ITEMS] = {};
	int m_itemCount = 0;

	Ptr<Texture2D> m_atlas = nullptr;

private:
	ItemDB() { m_instance = this; }

public:
	/// <summary>
	/// Initializes the item database.
	/// </summary>
	static void init();

	/// <summary>
	/// Generate all block texture atlases.
	/// </summary>
	static void generateAtlases();

	/// <summary>
	/// Registers item on given id.
	/// </summary>
	/// <param name="id">The item id.</param>
	/// <param name="item">The item data.</param>
	static void registerItem(itemid id, Item item);

	/// <summary>
	/// Registers block texture, and creates new item block data.
	/// </summary>
	/// <param name="flags"></param>
	/// <param name="id"></param>
	/// <param name="texture"></param>
	/// <param name="top_texture"></param>
	/// <param name="bottom_texture"></param>
	/// <param name="sideoverlay_texture"></param>
	/// <returns></returns>
	static Item_BlockData* createBlockData(
		itemid id,
		const char* texture, 
		const char* top_texture = nullptr,
		const char* bottom_texture = nullptr,
		const char* sideoverlay_texture = nullptr);

	/// <summary>
	/// Gets item with given id.
	/// </summary>
	/// <returns>The selected item, or null when not found(not defined).</returns>
	FORCEINLINE static Item* getItem(uint id)
	{
		if (m_instance->m_items[id].itemType == ItemType::unknown)
			return nullptr;

		return getItemUnsafe(id);
	}

	/// <summary>
	/// UNSAFE. Gets item with given id.
	/// </summary>
	/// <returns>The selected item.</returns>
	FORCEINLINE static Item* getItemUnsafe(uint id)
	{
		return &m_instance->m_items[id];
	}

	/// <summary>
	/// Gets current instance of item database.
	/// </summary>
	/// <returns>The current instance of item database.</returns>
	FORCEINLINE static ItemDB* getInstance()
	{
		return m_instance;
	}

	/// <summary>
	/// Shutdowns the item database.
	/// </summary>
	static void shutdown();
};

#endif // ITEMDB_H
