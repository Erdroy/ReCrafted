// ReCrafted (c) 2016-2018 Always Too Late

#include "Atlas.h"
#include "Common/Text.h"
#include "Platform/Platform.h"

#include <json.hpp>

void Atlas::Dispose()
{
    // Dispose texture
    m_texture = nullptr;
}

Rect Atlas::GetRect(const char* name)
{
    for (auto i = 0u; i < m_elements.Size(); i ++)
    {
        if (strcmp(m_elements[i].name, name) == 0)
        {
            return m_elements[i].rect;
        }
    }

    return {};
}

Ref<Texture2D> Atlas::GetTexture() const
{
    return m_texture;
}

Ref<Atlas> Atlas::Load(Text& fileName)
{
    char jsonName[512] = {};
    char pngName[512] = {};
    fileName.CStr(jsonName, 512);

    // build png filename
    strcpy_s(pngName, jsonName);
    auto len = strlen(pngName);
    pngName[len - 4] = 'p';
    pngName[len - 3] = 'n';
    pngName[len - 2] = 'g';
    pngName[len - 1] = '\0';

    if (Platform::FileExists(jsonName))
    {
        File jsonFile = {};
        Platform::OpenFile(&jsonFile, jsonName, OpenMode::OpenRead);

        // read data
        auto data = new char[jsonFile.FileSize];
        memset(data, 0, jsonFile.FileSize);
        jsonFile.Read(data, jsonFile.FileSize);

        auto json = nlohmann::json::parse(data);

        // create instance
        Ref<Atlas> atlas(new Atlas);

        // build element list
        for (auto i = 0u; i < json.size(); i ++)
        {
            atlas->m_elements.Add({});
            auto elem = &atlas->m_elements[atlas->m_elements.Size() - 1];

            auto name = json[i]["Name"].get<std::string>();
            strcpy(elem->name, name.c_str());

            auto x = json[i]["X"].get<int>();
            auto y = json[i]["Y"].get<int>();
            auto w = json[i]["Width"].get<int>();
            auto h = json[i]["Height"].get<int>();

            elem->rect = Rect(x, y, w, h);
        }

        // load texture
        auto texture = Texture2D::CreateTexture(Renderer::TextureFormat::RGBA8);
        texture->LoadFromFile(pngName);
        texture->Apply();

        // set loaded texture
        atlas->m_texture = texture;

        // free data
        delete[] data;

        return atlas;
    }

    return nullptr;
}
