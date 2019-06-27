// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

using System;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Content 
{
    /// <summary>
    ///     ContentManager class. Provides asset management functionality (loading etc.).
    /// </summary>
    public static partial class ContentManager
    {

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalInternalInitVirtualAsset(System.IntPtr asset);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalInternalLoadAssetSync(System.IntPtr asset, string assetFile);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalInternalLoadAssetAsync(System.IntPtr asset, string assetFile, System.Action<Asset> onLoad);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalInternalAddAssetLoadEvent(System.IntPtr asset, System.Action<Asset> onLoad);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Asset InternalInternalFindAsset(Guid guid);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Asset InternalInternalFindAsset(string assetFile);
    }
}