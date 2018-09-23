// ReCrafted (c) 2016-2018 Always Too Late

#include "ContentManager.h"
#include "TestAsset.h"
#include "Core/Streams/FileStream.h"
#include "Core/Logger.h"

SINGLETON_IMPL(ContentManager)

void ContentManager::OnInit()
{
    cvar asset = LoadAsset<TestAsset>(TEXT_CONST("testasset"));

}

void ContentManager::OnDispose()
{
}

void ContentManager::Update()
{
}

void ContentManager::RegisterAsset(AssetBase* asset)
{
    // TODO: Asset management
}

bool ContentManager::LoadAsset(AssetBase* asset, const Text& name) const
{
    // Build file name
    cvar assetName = name.StdStr();
    cvar assetFile = "../content/" + assetName + ".rcasset";

    // Check if file exists
    if(!Platform::FileExists(assetFile.c_str()))
    {
        Logger::LogError("Failed to load asset '{0}'. File '{1}' not found.", assetName.c_str(), assetFile.c_str());
        return true;
    }

#ifdef _DEBUG
    Logger::Log("Loading asset '{0}'", assetName.c_str());
#endif

    // Open streams
    var fileStream = FileStream(assetFile.c_str(), OpenMode::OpenRead);
    var binaryStream = BinaryStream(fileStream);

    ASSERT(fileStream.IsOpen());

    // Deserialize asset
    asset->Deserialize(binaryStream);

    // Dispose file stream
    fileStream.Dispose();
    return false;
}
