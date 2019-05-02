// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

using System.Runtime.CompilerServices;

namespace ReCrafted.API 
{
    public partial class Object
    {

        public static void Destroy(Object objectInstance)
        {
            InternalDestroy(objectInstance.NativePtr);
        }

        public static void DestroyNow(Object objectInstance)
        {
            InternalDestroyNow(objectInstance.NativePtr);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalDestroy(System.IntPtr objectInstance);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalDestroyNow(System.IntPtr objectInstance);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern object InternalNew(System.IntPtr type);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern object InternalNewGeneric(System.IntPtr baseType, System.IntPtr obj);
    }
}