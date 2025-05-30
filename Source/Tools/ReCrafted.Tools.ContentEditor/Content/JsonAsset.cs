﻿// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

namespace ReCrafted.Tools.ContentEditor.Content
{

    public abstract class JsonAsset : Asset
    {
        protected abstract override void OnSerializeJson(ushort version);
        protected abstract override void OnDeserializeJson(ushort version);

        protected override AssetBaseType AssetBaseType => AssetBaseType.Json;
    }
}
