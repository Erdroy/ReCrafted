// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Atlas.h"
#include "Platform/Platform.Windows.h"
#include "Common/Text.h"

#include <json.hpp>

void Atlas::init()
{

}

void Atlas::dispose()
{

}

Ptr<Atlas> Atlas::load(Text fileName)
{
	char jsonName[MAX_PATH] = {};
	char pngName[MAX_PATH] = {};
	fileName.c_str(jsonName, MAX_PATH);

	// build png filename
	strcpy_s(pngName, jsonName);
	auto len = strlen(pngName);
	pngName[len - 4] = 'p';
	pngName[len - 3] = 'n';
	pngName[len - 2] = 'g';
	pngName[len - 1] = '\0';

	if (Platform::fileExists(jsonName))
	{
		File jsonFile = {};
		Platform::openFile(&jsonFile, jsonName, OpenMode::OpenRead);

		// read data
		auto data = new char[jsonFile.FileSize];
		jsonFile.read(data, jsonFile.FileSize);

		auto json = nlohmann::json::parse(data);
		
		// TODO: create instance
		// TODO: build element list
		// TODO: load texture

		// free data
		delete[] data;

		// TODO: return!
	}

	return nullptr;
}
