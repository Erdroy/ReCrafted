// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

using ReCrafted.API.Core.Actors;
using ReCrafted.API.Physics;
using ReCrafted.API.Mathematics;
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
        ///     Adds given actor to the view list of this voxel object.
        ///     It will be used to make sure, that the terrain around the actor
        ///     is always loaded with proper LoD.
        /// </summary>
        /// <param name="viewActor">The actor.</param>
        /// <remarks>
        ///     Remember to remove the actor using RemoveViewActor(...) before it is destroyed.
        /// </remarks>
        public void AddViewActor(ActorBase viewActor)
        {
            InternalAddViewActor(NativePtr, viewActor? viewActor.NativePtr : System.IntPtr.Zero);
        }
        
        /// <summary>
        ///     Removes given actor from the view list of this voxel object.
        /// </summary>
        /// <param name="viewActor">The actor.</param>
        public void RemoveViewActor(ActorBase viewActor)
        {
            InternalRemoveViewActor(NativePtr, viewActor? viewActor.NativePtr : System.IntPtr.Zero);
        }
        
        /// <summary>
        ///     Modifies this voxel space object, using given method.
        /// </summary>
        /// <param name="material">The voxel material that will be applied on every voxel.</param>
        /// <param name="mode">The edit mode.</param>
        /// <param name="position">The edit position.</param>
        /// <param name="size">The edit size.</param>
        public void Modify(ushort material, VoxelEditMode mode, Vector3 position, float size)
        {
            InternalModify(NativePtr, material, mode, position, size);
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
        ///     Gets the RigidBody actor that is representing this VoxelObject's physics state and collision.
        /// </summary>
        public RigidBodyActor RigidBody
        {
            get
            {
                Get_InternalRigidBody(NativePtr, out var data);
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
        private static extern void InternalAddViewActor(System.IntPtr nativePtr, System.IntPtr viewActor);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalRemoveViewActor(System.IntPtr nativePtr, System.IntPtr viewActor);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalModify(System.IntPtr nativePtr, ushort material, VoxelEditMode mode, Vector3 position, float size);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalRadius(System.IntPtr nativePtr, out double data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalAsset(System.IntPtr nativePtr, out VoxelObjectAsset data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalRigidBody(System.IntPtr nativePtr, out RigidBodyActor data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalIsLoading(System.IntPtr nativePtr, out bool data);
    }
}