// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

using ReCrafted.API.Mathematics;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Physics 
{
    /// <summary>
    ///     BoxCollider class.
    /// </summary>
    public sealed class BoxCollider : Collider
    {

        /// <summary>
        ///     Gets or sets the size of this box collider.
        /// </summary>
        public Vector3 Size
        {
            get
            {
                Get_InternalSize(NativePtr, out var data);
                return data;
            }
            set => Set_InternalSize(NativePtr, ref value);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalSize(System.IntPtr nativePtr, out Vector3 data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalSize(System.IntPtr nativePtr, ref Vector3 data);
    }
}