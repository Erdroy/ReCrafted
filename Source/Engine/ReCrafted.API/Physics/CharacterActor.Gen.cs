// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

using ReCrafted.API.Core.Actors;
using ReCrafted.API.Mathematics;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Physics 
{
    /// <summary>
    ///     RigidBodyActor actor class.
    /// </summary>
    public sealed class CharacterActor : ActorBase
    {
        
        /// <summary>
        ///     Moves this character along given displacement vector.
        /// </summary>
        /// <param name="displacement">The displacement vector.</param>
        /// <returns>The collision flags.</returns>
        public CharacterCollisionFlags Move(Vector3 displacement)
        {
            return InternalMove(NativePtr, displacement);
        }

        /// <summary>
        ///     Gets or sets the collision layer.
        /// </summary>
        public uint CollisionLayer
        {
            get
            {
                Get_InternalCollisionLayer(NativePtr, out var data);
                return data;
            }
            set => Set_InternalCollisionLayer(NativePtr, ref value);
        }

        /// <summary>
        ///     Gets or sets the up direction.
        /// </summary>
        public Vector3 UpDirection
        {
            get
            {
                Get_InternalUpDirection(NativePtr, out var data);
                return data;
            }
            set => Set_InternalUpDirection(NativePtr, ref value);
        }

        /// <summary>
        ///     Gets or sets the height.
        /// </summary>
        public float Height
        {
            get
            {
                Get_InternalHeight(NativePtr, out var data);
                return data;
            }
            set => Set_InternalHeight(NativePtr, ref value);
        }

        /// <summary>
        ///     Gets or sets the radius.
        /// </summary>
        public float Radius
        {
            get
            {
                Get_InternalRadius(NativePtr, out var data);
                return data;
            }
            set => Set_InternalRadius(NativePtr, ref value);
        }

        /// <summary>
        ///     Gets or sets the step offset.
        /// </summary>
        public float StepOffset
        {
            get
            {
                Get_InternalStepOffset(NativePtr, out var data);
                return data;
            }
            set => Set_InternalStepOffset(NativePtr, ref value);
        }

        /// <summary>
        ///     Gets or sets the contact offset.
        /// </summary>
        public float ContactOffset
        {
            get
            {
                Get_InternalContactOffset(NativePtr, out var data);
                return data;
            }
            set => Set_InternalContactOffset(NativePtr, ref value);
        }

        /// <summary>
        ///     Gets or sets the slope limit.
        /// </summary>
        public float SlopeLimit
        {
            get
            {
                Get_InternalSlopeLimit(NativePtr, out var data);
                return data;
            }
            set => Set_InternalSlopeLimit(NativePtr, ref value);
        }

        /// <summary>
        ///     Gets the velocity.
        /// </summary>
        public Vector3 Velocity
        {
            get
            {
                Get_InternalVelocity(NativePtr, out var data);
                return data;
            }
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern CharacterCollisionFlags InternalMove(System.IntPtr nativePtr, Vector3 displacement);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalCollisionLayer(System.IntPtr nativePtr, out uint data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalCollisionLayer(System.IntPtr nativePtr, ref uint data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalUpDirection(System.IntPtr nativePtr, out Vector3 data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalUpDirection(System.IntPtr nativePtr, ref Vector3 data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalHeight(System.IntPtr nativePtr, out float data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalHeight(System.IntPtr nativePtr, ref float data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalRadius(System.IntPtr nativePtr, out float data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalRadius(System.IntPtr nativePtr, ref float data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalStepOffset(System.IntPtr nativePtr, out float data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalStepOffset(System.IntPtr nativePtr, ref float data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalContactOffset(System.IntPtr nativePtr, out float data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalContactOffset(System.IntPtr nativePtr, ref float data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalSlopeLimit(System.IntPtr nativePtr, out float data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalSlopeLimit(System.IntPtr nativePtr, ref float data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalVelocity(System.IntPtr nativePtr, out Vector3 data);
    }
}