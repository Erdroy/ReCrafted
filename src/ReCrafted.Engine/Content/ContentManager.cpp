// ReCrafted (c) 2016-2018 Always Too Late

#include "ContentManager.h"
#include "TestAsset.h"
#include "Core/Streams/FileStream.h"

SINGLETON_IMPL(ContentManager)

void ContentManager::OnInit()
{
    cvar asset = LoadAsset<TestAsset>(TEXT_CONST("../content/testasset.rcasset"));

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

void ContentManager::LoadAsset(AssetBase* asset, const Text& assetFile)
{
    var fileStream = FileStream(assetFile.StdStr().c_str(), OpenMode::OpenRead);
    var binaryStream = BinaryStream(fileStream);

    ASSERT(fileStream.IsOpen());

    // Deserialize asset
    asset->Deserialize(binaryStream);

    // Dispose file stream
    fileStream.Dispose();
}
