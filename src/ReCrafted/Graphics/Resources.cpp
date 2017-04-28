// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Resources.h"
#include "../Utils/Types.h"

#define MAX_ASSET_TEXTURE2D 16 << 10
Ptr<Texture2D> m_textures[MAX_ASSET_TEXTURE2D];
char m_textures_names[16][MAX_ASSET_TEXTURE2D];

#define MAX_ASSET_SHADER 128
Ptr<Shader> m_shaders[MAX_ASSET_SHADER];
char m_shaders_names[16][MAX_ASSET_SHADER];


#define ASSET_GET_FREE(array, max) \
for(auto i = 0; i < max; i ++) { \
if (array[i] == nullptr) { \
idx = i; \
break;}}

Ptr<Texture2D> Resources::getTexture(const char* name)
{
	for(auto i = 0; i < MAX_ASSET_TEXTURE2D; i++)
	{
		if(strcmp(m_textures_names[i], name) == 0)
		{
			return m_textures[i];
		}
	}

	return nullptr;
}

Ptr<Shader> Resources::getShader(const char* name)
{
	for (auto i = 0; i < MAX_ASSET_SHADER; i++)
	{
		if (strcmp(m_shaders_names[i], name) == 0)
		{
			return m_shaders[i];
		}
	}

	return nullptr;
}

void Resources::loadTexture(const char* fileName, const char* name, uint flags)
{
	auto idx = -1;
	ASSET_GET_FREE(m_textures, MAX_ASSET_TEXTURE2D);

	if (idx < 0)
		return;

	m_textures[idx] = Texture2D::createTexture();
	m_textures[idx]->loadFile(fileName, flags);
	strcpy_s(m_textures_names[idx], name);
}

void Resources::loadShader(const char* fileName, const char* name)
{
	auto idx = -1;
	ASSET_GET_FREE(m_shaders, MAX_ASSET_SHADER);

	if (idx < 0)
		return;

	m_shaders[idx] = Shader::loadShader(fileName);
	strcpy_s(m_shaders_names[idx], name);
}

void Resources::release()
{
}
