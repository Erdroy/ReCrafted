// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef ITEM_H
#define ITEM_H

// includes
#include "../../recraftedPrerequisites.h"
#include "../../Common/Text.h"
#include "../../Graphics/Texture2D.h"

struct ItemType
{
	enum Enum
	{
		unknown,
		block,

		count
	};
};

struct Item
{
public:
	ItemType::Enum itemType = ItemType::unknown;
	Text name = {};
	Text description = {};
	Ptr<Texture2D> icon = nullptr;
};

#endif // ITEM_H
