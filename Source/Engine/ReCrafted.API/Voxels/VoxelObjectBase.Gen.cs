// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

using System.Runtime.CompilerServices;

namespace ReCrafted.API.Voxels 
{
    /// <summary>
    ///     The base class of all voxel objects (planets, asteroids etc.).
    /// </summary>
    public abstract partial class VoxelObjectBase : Object
    {
        
        public virtual void Initialize()
        {
            InternalInitialize(NativePtr);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalInitialize(System.IntPtr nativePtr);
    }
}