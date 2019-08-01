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

        /// <summary>
        ///     The radius of this voxel object.
        /// </summary>
        public double Radius
        {
            get
            {
                Get_InternalRadius(NativePtr, out var data);
                return data;
            }
        }

        /// <summary>
        ///     The asset of this voxel object.
        /// </summary>
        public VoxelObjectAsset Asset
        {
            get
            {
                Get_InternalAsset(NativePtr, out var data);
                return data;
            }
        }

        /// <summary>
        ///     Gets the loading state. True when this voxel object is still loading the initial data.
        /// </summary>
        public bool IsLoading
        {
            get
            {
                Get_InternalIsLoading(NativePtr, out var data);
                return data;
            }
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalInitialize(System.IntPtr nativePtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalRadius(System.IntPtr nativePtr, out double data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalAsset(System.IntPtr nativePtr, out VoxelObjectAsset data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalIsLoading(System.IntPtr nativePtr, out bool data);
    }
}