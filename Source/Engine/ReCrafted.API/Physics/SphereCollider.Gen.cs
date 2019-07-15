// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

using ReCrafted.API.Mathematics;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Physics 
{
    /// <summary>
    ///     SphereCollider class.
    /// </summary>
    public sealed class SphereCollider : Collider
    {

        /// <summary>
        ///     Gets or sets the radius of this sphere collider.
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

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalRadius(System.IntPtr nativePtr, out float data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalRadius(System.IntPtr nativePtr, ref float data);
    }
}