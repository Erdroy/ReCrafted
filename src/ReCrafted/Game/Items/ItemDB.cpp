// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "ItemDB.h"

ItemDB* ItemDB::m_instance;

void ItemDB::init()
{
	m_instance = new ItemDB;
}

void ItemDB::shutdown()
{
	if (m_instance == nullptr)
		return;

	delete m_instance;
}
