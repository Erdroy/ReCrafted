// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

using ReCrafted.API.Content;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Voxels 
{
    public sealed class VoxelObjectAsset : BinaryAsset
    {

        /// <summary>
        ///     Name of this VoxelObject.
        /// </summary>
        public string ObjectName
        {
            get
            {
                Get_InternalObjectName(NativePtr, out var data);
                return data;
            }
        }

        /// <summary>
        ///     The VoxelObject type.
        /// </summary>
        public VoxelObjectType ObjectType
        {
            get
            {
                Get_InternalObjectType(NativePtr, out var data);
                return data;
            }
        }

        /// <summary>
        /// The minimal surface height.
        /// </summary>
        public uint MinimumSurfaceHeight
        {
            get
            {
                Get_InternalMinimumSurfaceHeight(NativePtr, out var data);
                return data;
            }
        }

        /// <summary>
        ///     The maximal surface height. This is also the maximal build height.
        /// </summary>
        public uint MaximumSurfaceHeight
        {
            get
            {
                Get_InternalMaximumSurfaceHeight(NativePtr, out var data);
                return data;
            }
        }

        /// <summary>
        ///     The maximal height of hills.
        /// </summary>
        public uint HillsHeight
        {
            get
            {
                Get_InternalHillsHeight(NativePtr, out var data);
                return data;
            }
        }

        /// <summary>
        ///     The initial depth of the VoxelObject's octree.
        /// </summary>
        public uint InitialOctreeDepth
        {
            get
            {
                Get_InternalInitialOctreeDepth(NativePtr, out var data);
                return data;
            }
        }

        /// <summary>
        ///     Gets the voxel object from this asset.
        /// </summary>
        public VoxelObjectBase VoxelObject
        {
            get
            {
                Get_InternalVoxelObject(NativePtr, out var data);
                return data;
            }
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalObjectName(System.IntPtr nativePtr, out string data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalObjectType(System.IntPtr nativePtr, out VoxelObjectType data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalMinimumSurfaceHeight(System.IntPtr nativePtr, out uint data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalMaximumSurfaceHeight(System.IntPtr nativePtr, out uint data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalHillsHeight(System.IntPtr nativePtr, out uint data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalInitialOctreeDepth(System.IntPtr nativePtr, out uint data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalVoxelObject(System.IntPtr nativePtr, out VoxelObjectBase data);
    }
}