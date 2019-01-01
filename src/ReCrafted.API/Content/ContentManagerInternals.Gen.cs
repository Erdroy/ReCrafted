// ReCrafted (c) 2016-2019 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 10/26/2018 22:37:46 Source: 'Content.API.cpp' Target: 'Content/ContentManagerInternals.Gen.cs'

using System;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Content 
{
	/// <summary>
	///	ContentManagerInternals class.
	/// </summary>
	internal static class ContentManagerInternals 
	{
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void UnloadAsset(IntPtr assetNativePtr);

		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern Asset CreateVirtual(IntPtr assetNativePtr);

		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern Asset LoadAsset(IntPtr assetNativePtr, string assetFile);

		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void LoadAssetAsync(IntPtr assetNativePtr, string assetFile, AssetLoadCallback onLoad);

	}
}
