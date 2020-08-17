// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

using System;

namespace ReCrafted.Tools.ContentEditor.Content
{
    public interface IAssetMetadata
    {
        string TargetDirectory { get; set; }
        AssetType AssetType { get; set; }
        Guid AssetGuid { get; set; }
    }
}
