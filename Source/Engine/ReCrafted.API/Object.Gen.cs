// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

using System.Runtime.CompilerServices;

namespace ReCrafted.API 
{
    /// <summary>
    ///     Object class. Base class for all mono object instances.
    /// </summary>
    public partial class Object
    {
        
        /// <summary>
        ///     Destroys given Object instance.
        /// </summary>
        public static void Destroy(Object objectInstance)
        {
            InternalDestroy(objectInstance? objectInstance.NativePtr : System.IntPtr.Zero);
        }
        
        /// <summary>
        ///     Destroys given Object instance now.
        /// </summary>
        public static void DestroyNow(Object objectInstance)
        {
            InternalDestroyNow(objectInstance? objectInstance.NativePtr : System.IntPtr.Zero);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalDestroy(System.IntPtr objectInstance);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalDestroyNow(System.IntPtr objectInstance);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern object InternalNew(System.IntPtr type);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern object InternalNewGeneric(System.IntPtr baseType, System.IntPtr type, object obj);
    }
}