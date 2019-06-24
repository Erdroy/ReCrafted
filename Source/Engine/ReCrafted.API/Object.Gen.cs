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
        ///     Adds single reference count to the object's refCount variable.
        /// </summary>
        internal void AddRef()
        {
            InternalAddRef(NativePtr);
        }
        
        /// <summary>
        ///     Removes single reference from the object's refCount variable and returns true when it's refCount is 0.
        /// </summary>
        internal bool RemoveRef()
        {
            return InternalRemoveRef(NativePtr);
        }
        
        /// <summary>
        ///     Returns the ref count of this object.
        /// </summary>
        /// <returns>This object's reference count.</returns>
        internal uint GetRefCount()
        {
            return InternalGetRefCount(NativePtr);
        }
        
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
        private static extern void InternalAddRef(System.IntPtr nativePtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool InternalRemoveRef(System.IntPtr nativePtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint InternalGetRefCount(System.IntPtr nativePtr);

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