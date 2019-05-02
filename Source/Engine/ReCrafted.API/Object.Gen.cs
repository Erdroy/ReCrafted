// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

using System.Runtime.CompilerServices;

namespace ReCrafted.API 
{
    public partial class Object
    {

        public static void Destroy(Object objectInstance)
        {
            InternalDestroy(objectInstance);
        }

        public static void DestroyNow(Object objectInstance)
        {
            InternalDestroyNow(objectInstance);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalDestroy(Object objectInstance);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalDestroyNow(Object objectInstance);
    }
}