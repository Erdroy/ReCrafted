// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

using ReCrafted.API.Core.Actors;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Voxels 
{
    /// <summary>
    ///     VoxelChunk actor class.
    /// </summary>
    public sealed class VoxelChunk : ActorBase
    {

        /// <summary>
        ///     Gets or sets the chunk's visibility state. Invisible when true.
        /// </summary>
        public bool Visible
        {
            get
            {
                Get_InternalVisible(NativePtr, out var data);
                return data;
            }
            set => Set_InternalVisible(NativePtr, ref value);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalVisible(System.IntPtr nativePtr, out bool data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalVisible(System.IntPtr nativePtr, ref bool data);
    }
}