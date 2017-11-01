// ReCrafted © 2016-2017 Always Too Late

#pragma once

#ifndef ITEM_H
#define ITEM_H

// includes
#include "Common/Text.h"
#include "Graphics/Texture2D.h"

struct ItemType
{
	enum Enum
	{
		unknown,
		block,

		count
	};
};

struct ItemData { };
struct ItemStats { };

struct Item
{
public:
	ItemType::Enum itemType = ItemType::unknown;
	Text name = {};
	Text description = {};
	Ptr<Texture2D> icon = nullptr;
	ItemData* data = nullptr;
	ItemStats* stats = nullptr;

public:
	void dispose()
	{
		if (data) {
			delete data;
			data = nullptr;
		}
		if (stats) {
			delete stats;
			stats = nullptr;
		}
	}
};

#define ITEM_BLOCKFLAG_NONE				UINT8_C(0x000) // no flags.
#define ITEM_BLOCKFLAG_HAS_SIDEOVERLAY	UINT8_C(0x001) // This has side overlay, eg. a grass
#define ITEM_BLOCKFLAG_HAS_DIFF_TOP		UINT8_C(0x002) // This has different top texture, eg. a grass
#define ITEM_BLOCKFLAG_HAS_DIFF_BOTTOM	UINT8_C(0x004) // This has different bottom texture, like minecraft sandstone

struct Item_BlockData : ItemData
{
public:
	byte flags = 0u;

	float texture_u = 0.0f;
	float texture_v = 0.0f;
	float texture_w = 0.0f;
	float texture_h = 0.0f;

	float texture_top_u = 0.0f;
	float texture_top_v = 0.0f;
	float texture_top_w = 0.0f;
	float texture_top_h = 0.0f;

	float texture_bottom_u = 0.0f;
	float texture_bottom_v = 0.0f;
	float texture_bottom_w = 0.0f;
	float texture_bottom_h = 0.0f;
};

#endif // ITEM_H
