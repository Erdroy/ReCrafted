// ReCrafted (c) 2016-2018 Always Too Late

using System;
using ReCrafted.API.Core;

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
        /// Destroys this object.
        /// </summary>
        public void Destroy()
        {
            Destroy(this);
        }

        /// <summary>
        /// Creates managed Object of given type with it's unmanaged corresponsive type.
        /// </summary>
        /// <typeparam name="TType">The type of Object to be created.</typeparam>
        /// <returns>The created Object.</returns>
        public static TType New<TType>() where TType : Object
        {
            // Do not allow to create raw Object files.
            if (typeof(TType) == typeof(Object))
                throw new ReCraftedException("Cannot create instance of raw Object type!");

            // Create managed and unmanaged object
            return (TType)InternalNew(typeof(TType).TypeHandle.Value);
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
