// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

using System.Runtime.CompilerServices;

namespace ReCrafted.API.Voxels 
{
    /// <summary>
    ///     VoxelPlanet class.
    ///     This is class that is being used by every voxel planet.
    ///     Uses cube height map for the voxel generation and octree population with dynamic LoD.
    /// </summary>
    public sealed partial class VoxelPlanet : VoxelObjectBase
    {
        
        public override void Initialize()
        {
            InternalInitialize(NativePtr);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalInitialize(System.IntPtr nativePtr);
    }
}