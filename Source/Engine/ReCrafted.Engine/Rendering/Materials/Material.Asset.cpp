// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "Material.h"
#include "Content/ContentManager.h"

MaterialFieldType Material::ParseFieldType(const std::string& type)
{
    if (type == "float")
        return MaterialFieldType::Float1;

    if (type == "float2")
        return MaterialFieldType::Float2;

    if (type == "float3")
        return MaterialFieldType::Float3;
    
    if (type == "float4")
        return MaterialFieldType::Float4;

    if (type == "color32")
        return MaterialFieldType::Color32;

    return MaterialFieldType::Unknown;
}

void Material::OnDeserializeJson(uint16_t version, const json& json)
{
    // Setup material name
    const auto mName = json["MaterialName"];
    ASSERT(!mName.is_null());
    ASSERT(mName.is_string());
    m_name = mName.get<std::string>();

    // Setup shader asset file
    const auto mShader = json["MaterialShader"];
    ASSERT(!mShader.is_null());
    ASSERT(mShader.is_string());
    m_shaderAssetName = mShader.get<std::string>();

    // Load textures
    for(auto&& textureArrayItem : json["Textures"])
    {
        // Do not allow non-specified asset names
        // This would throw exception in the ContentManager and also adding null-texture would
        // end with an undefined-behaviour in the rendering pipeline (but more likely would end with an exception).
        ASSERT(!textureArrayItem.is_null());
        ASSERT(textureArrayItem.is_string());

        // Get texture asset name
        const auto assetName = textureArrayItem.get<std::string>();
        ASSERT(!assetName.empty());

        // We cannot use the ContentManager here, because we are running on task worker thread.
        // There is no need to lock the texture queue list as it will not be accessed by any other thread at this moment.
        m_textureLoadQueue.Add(assetName);
    }

    // Load fields
    for (auto&& fieldArrayItem : json["Fields"])
    {
        ASSERT(!fieldArrayItem.is_null());

        const auto fName = fieldArrayItem["Name"];
        ASSERT(!fName.is_null());
        ASSERT(fName.is_string());

        const auto fType = fieldArrayItem["Type"];
        ASSERT(!fType.is_null());
        ASSERT(fType.is_string());

        const auto fValue = fieldArrayItem["Value"];

        // Setup field fields... And it's data.
        auto field = new MaterialField();
        field->Name = fName.get<std::string>();

        const auto fTypeEnum = ParseFieldType(fType);

        switch(fTypeEnum)
        {
        case MaterialFieldType::Float:
        { 
            field->Size = sizeof(float);
            field->Data = new uint8_t[field->Size];
            const auto value = fValue.get<float>();
            *reinterpret_cast<float*>(field->Data) = value;
            break;
        }
        case MaterialFieldType::Float2:
        {
            field->Size = sizeof(float) * 2;
            auto data = field->Data = new uint8_t[field->Size];

            for (auto i = 0; i < 2; i++)
            {
                const auto value = fValue[i].get<float>();
                *reinterpret_cast<float*>(data) = value;
                data += sizeof(float);
            }
            break;
        }
        case MaterialFieldType::Float3:
        {
            field->Size = sizeof(float) * 3;
            auto data = field->Data = new uint8_t[field->Size];

            for (auto i = 0; i < 3; i++)
            {
                const auto value = fValue[i].get<float>();
                *reinterpret_cast<float*>(data) = value;
                data += sizeof(float);
            }
            break;
        }
        case MaterialFieldType::Float4:
        {
            field->Size = sizeof(float) * 4;
            auto data = field->Data = new uint8_t[field->Size];

            for (auto i = 0; i < 4; i++)
            {
                const auto value = fValue[i].get<float>();
                *reinterpret_cast<float*>(data) = value;
                data += sizeof(float);
            }
            break;
        }

        case MaterialFieldType::Unknown:
        case MaterialFieldType::Count:
        default:
            Logger::LogError("Unknown material field type '{0}'.", fType.get<std::string>());
            ASSERT(false);
            break;
        }

        // Add field to the list
        // Note: Fields are uploaded to the shader constant buffer by the order they are 
        // in this `m_fields` list.
        m_fields.Add(field);
    }
}

void Material::OnLoadEnd()
{
    // Load shader
    ASSERT(!m_shaderAssetName.empty());
    m_shader = ContentManager::LoadAsset<Shader>(m_shaderAssetName.c_str());
    m_shaderAssetName.clear();

    // Load textures
    for(auto&& textureAssetName : m_textureLoadQueue)
    {
        ASSERT(!textureAssetName.empty());

        // Load texture (this will start loading the texture), at first, we will get only first mip.
        const auto textureAsset = ContentManager::LoadAsset<Texture>(textureAssetName.c_str());

        // Make sure that we have got the asset.
        ASSERT(textureAsset);

        m_textures.Add(textureAsset);
    }

    // Clear texture load queue
    m_textureLoadQueue.Clear();
}

void Material::OnUnload()
{
    // Destroy shader
    Destroy(m_shader);

    // Destroy textures
    for(auto&& texture : m_textures)
        Destroy(texture);
    m_textures.Clear();

    // Release fields
    for (auto&& field : m_fields)
        delete field;
    m_fields.Clear();
}
