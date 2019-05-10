// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using System;
using System.Diagnostics;
using ReCrafted.API.Core;
using JetBrains.Annotations;

namespace ReCrafted.API
{
    /// <summary>
    ///     Object class. Base class for all objects.
    /// </summary>
    public partial class Object
    {
        /// <summary>
        ///     The native pointer for mono object instance.
        ///     This is being set by Object instantiation on the C++ side.
        /// </summary>
        [NonSerialized, UsedImplicitly]
        internal IntPtr NativePtr;

        /// <summary>
        ///     Default object constructor.
        /// </summary>
        protected Object() { }

        /// <summary>
        ///     Called when the object is finalized.
        /// </summary>
        ~Object()
        {
            if (NativePtr != IntPtr.Zero)
            {
                Destroy(this);
                NativePtr = IntPtr.Zero;
            }
        }

        /// <summary>
        ///     Destroys this object.
        /// </summary>
        public void Destroy()
        {
            Destroy(this);
        }


        /// <summary>
        ///     Returns true when valid.
        /// </summary>
        public bool IsValid()
        {
            return NativePtr != IntPtr.Zero;
        }

        /// <summary>
        ///     Creates managed Object of given type with it's unmanaged corresponsive type.
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
        ///     Creates generic managed Object of given type with it's unmanaged corresponsive type.
        /// </summary>
        /// <typeparam name="TType">The type of Object to be created.</typeparam>
        /// <returns>The created Object.</returns>
        public static TType NewGeneric<TType>(Type baseType) where TType : Object, new()
        {
            // Create new managed instance
            var instance = new TType();

            if(typeof(TType).BaseType == null)
                throw new ReCraftedException("The `TType` is not derived from any suitable Object children class!");

            // Do not allow to create raw Object files.
            if (typeof(TType) == typeof(Object))
                throw new ReCraftedException("Cannot create instance of raw Object type!");

            // Create managed and unmanaged object
            return (TType)InternalNewGeneric(baseType.TypeHandle.Value, typeof(TType).TypeHandle.Value, instance);
        }

        /// <summary>
        ///     Checks if object exists.
        /// </summary>
        /// <param name="obj">Object to check.</param>
        public static implicit operator bool(Object obj)
        {
            return obj != null && obj.NativePtr != IntPtr.Zero;
        }
    }
}
