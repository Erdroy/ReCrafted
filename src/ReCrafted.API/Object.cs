// ReCrafted (c) 2016-2018 Always Too Late

using System;

namespace ReCrafted.API
{
    /// <summary>
    /// Base class for all mono object instances.
    /// </summary>
    public partial class Object
    {
        /// <summary>
        /// The native pointer for mono object instance.
        /// This is being set by Object instantiation on the C++ side.
        /// </summary>
        [NonSerialized]
        internal IntPtr NativePtr;

        /// <summary>
        /// Default object constructor.
        /// </summary>
        protected Object() { }
        
        /// <summary>
        /// Called when the object is finalized.
        /// </summary>
        ~Object()
        {
            if (NativePtr != IntPtr.Zero)
            {
                // object was finalized
                InternalObjectFinalized(NativePtr);
            }
        }

        /// <summary>
        /// Destroy the given object.
        /// </summary>
        /// <param name="obj">The object instance which will be destroyed.</param>
        public static void Destroy(Object obj)
        {
            if (obj)
            {
                InternalDestroy(obj.NativePtr);
            }
        }
        
        /// <summary>
        /// Checks if object exists.
        /// </summary>
        /// <param name="obj">Object to check.</param>
        public static implicit operator bool(Object obj)
        {
            return obj != null && obj.NativePtr != IntPtr.Zero;
        }
    }
}
