// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Atlas.h"
#include "Platform/Platform.Windows.h"
#include "Common/Text.h"

#include <json.hpp>

void Atlas::dispose()
{
	// dispose texture
	SafeDispose(m_texture);
}

Rect Atlas::getRect(const char* name)
{
	for(auto i = 0u; i < m_elements.size(); i ++)
	{
		if(strcmp(m_elements[i].name, name) == 0)
		{
			return m_elements[i].rect;
		}
	}

	throw "Not found";
}

Ptr<Texture2D> Atlas::getTexture() const
{
	return m_texture;
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
		memset(data, 0, jsonFile.FileSize);
		jsonFile.read(data, jsonFile.FileSize);

		auto json = nlohmann::json::parse(data);

		// create instance
		Ptr<Atlas> atlas(new Atlas);

		// build element list
		for(auto i = 0u; i < json.size(); i ++)
		{
			atlas->m_elements.push_back({});
			auto elem = &atlas->m_elements[atlas->m_elements.size() - 1];

			auto name = json[i]["Name"].get<std::string>();
			strcpy(elem->name, name.c_str());

			auto x = json[i]["X"].get<int>();
			auto y = json[i]["Y"].get<int>();
			auto w = json[i]["Width"].get<int>();
			auto h = json[i]["Height"].get<int>();

			elem->rect = Rect(x, y, w, h);
		}

		// load texture
		auto texture = Texture2D::createTexture();
		texture->loadFile(pngName);
		texture->apply();

		// set loaded texture
		atlas->m_texture = texture;

		// free data
		delete[] data;

		return atlas;
	}

	return nullptr;
}
