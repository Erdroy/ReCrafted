// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

using System.Runtime.CompilerServices;

namespace ReCrafted.API.Physics 
{
    /// <summary>
    ///     PhysicsMaterial class.
    /// </summary>
    public sealed class PhysicsMaterial : Object
    {

        /// <summary>
        ///     Gets or sets the static friction material value. (default: 0.7)
        /// </summary>
        public float StaticFriction
        {
            get
            {
                Get_InternalStaticFriction(NativePtr, out var data);
                return data;
            }
            set => Set_InternalStaticFriction(NativePtr, ref value);
        }

        /// <summary>
        ///     Gets or sets the dynamic friction material value. (default: 0.7)
        /// </summary>
        public float DynamicFriction
        {
            get
            {
                Get_InternalDynamicFriction(NativePtr, out var data);
                return data;
            }
            set => Set_InternalDynamicFriction(NativePtr, ref value);
        }

        /// <summary>
        ///     Gets or sets the restitution material value. (default: 0.25)
        /// </summary>
        public float Restitution
        {
            get
            {
                Get_InternalRestitution(NativePtr, out var data);
                return data;
            }
            set => Set_InternalRestitution(NativePtr, ref value);
        }

        /// <summary>
        ///     Gets the default physics material.
        /// </summary>
        public static PhysicsMaterial Default
        {
            get
            {
                Get_InternalDefault(out var data);
                return data;
            }
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalStaticFriction(System.IntPtr nativePtr, out float data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalStaticFriction(System.IntPtr nativePtr, ref float data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalDynamicFriction(System.IntPtr nativePtr, out float data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalDynamicFriction(System.IntPtr nativePtr, ref float data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalRestitution(System.IntPtr nativePtr, out float data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalRestitution(System.IntPtr nativePtr, ref float data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalDefault(out PhysicsMaterial data);
    }
}