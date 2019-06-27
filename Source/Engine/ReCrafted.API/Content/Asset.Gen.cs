// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

using System;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Content 
{
    /// <summary>
    ///     Asset class. Base class for all assets.
    /// </summary>
    public abstract partial class Asset : Object
    {

        /// <summary>
        ///     Gets the original asset name.
        /// </summary>
        public string AssetName
        {
            get
            {
                Get_InternalAssetName(NativePtr, out var data);
                return data;
            }
        }

        /// <summary>
        ///     Gets source asset file that has been used to load this asset.
        /// </summary>
        public string AssetFile
        {
            get
            {
                Get_InternalAssetFile(NativePtr, out var data);
                return data;
            }
        }

        /// <summary>
        ///     Gets asset virtual flag state. True when this asset has been created using CreateVirtualAsset function.
        ///     This means that this asset was never loaded, and has no any representation in content.
        /// </summary>
        public bool IsVirtual
        {
            get
            {
                Get_InternalIsVirtual(NativePtr, out var data);
                return data;
            }
        }

        /// <summary>
        ///     Gets the asset load flag state. True when this asset has been loaded or it is virtual asset.
        /// </summary>
        /// <remarks>Can be called only from the MainThread.</remarks>
        public virtual bool IsLoaded
        {
            get
            {
                Get_InternalIsLoaded(NativePtr, out var data);
                return data;
            }
        }

        /// <summary>
        ///     Gets the asset loading state. True when still loading.
        /// </summary>
        /// <remarks>Can be called only from the MainThread.</remarks>
        public bool IsLoading
        {
            get
            {
                Get_InternalIsLoading(NativePtr, out var data);
                return data;
            }
        }

        /// <summary>
        ///     Gets the asset's guid.
        /// </summary>
        public Guid GetGuid
        {
            get
            {
                Get_InternalGetGuid(NativePtr, out var data);
                return data;
            }
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalAssetName(System.IntPtr nativePtr, out string data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalAssetFile(System.IntPtr nativePtr, out string data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalIsVirtual(System.IntPtr nativePtr, out bool data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalIsLoaded(System.IntPtr nativePtr, out bool data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalIsLoading(System.IntPtr nativePtr, out bool data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalGetGuid(System.IntPtr nativePtr, out Guid data);
    }
}