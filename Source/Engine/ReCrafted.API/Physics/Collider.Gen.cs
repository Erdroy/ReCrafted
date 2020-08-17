// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

using ReCrafted.API.Mathematics;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Physics 
{
    /// <summary>
    ///     Collider base class.
    /// </summary>
    public class Collider : Object
    {

        /// <summary>
        ///     Gets or sets the local position offset of this box collider.
        /// </summary>
        public Vector3 LocalPosition
        {
            get
            {
                Get_InternalLocalPosition(NativePtr, out var data);
                return data;
            }
            set => Set_InternalLocalPosition(NativePtr, ref value);
        }

        /// <summary>
        ///     Gets or sets the local rotation of this box collider.
        /// </summary>
        public Quaternion LocalRotation
        {
            get
            {
                Get_InternalLocalRotation(NativePtr, out var data);
                return data;
            }
            set => Set_InternalLocalRotation(NativePtr, ref value);
        }

        /// <summary>
        ///     Gets or sets the physics material of this collider.
        /// </summary>
        public PhysicsMaterial Material
        {
            get
            {
                Get_InternalMaterial(NativePtr, out var data);
                return data;
            }
            set => Set_InternalMaterial(NativePtr, value? value.NativePtr : System.IntPtr.Zero);
        }

        /// <summary>
        ///     Gets the valid state of this collider. True when this collider is valid and can be attached to rigid bodies.
        /// </summary>
        public bool IsValid
        {
            get
            {
                Get_InternalIsValid(NativePtr, out var data);
                return data;
            }
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalLocalPosition(System.IntPtr nativePtr, out Vector3 data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalLocalPosition(System.IntPtr nativePtr, ref Vector3 data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalLocalRotation(System.IntPtr nativePtr, out Quaternion data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalLocalRotation(System.IntPtr nativePtr, ref Quaternion data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalMaterial(System.IntPtr nativePtr, out PhysicsMaterial data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalMaterial(System.IntPtr nativePtr, System.IntPtr data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalIsValid(System.IntPtr nativePtr, out bool data);
    }
}