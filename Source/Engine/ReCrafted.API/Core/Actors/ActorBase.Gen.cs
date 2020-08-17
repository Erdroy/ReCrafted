// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

using ReCrafted.API.Common;
using ReCrafted.API.Mathematics;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Core.Actors 
{
    /// <summary>
    ///     Base class for all actors. Provides basic actor behavior.
    /// </summary>
    /// <remarks>
    ///     Actors can be defined only in C++ source code.
    /// </remarks>
    public abstract partial class ActorBase : Object
    {
        
        /// <summary>
        ///     Sets given actor as parent of this actor.
        /// </summary>
        /// <param name="newParent">The parent instance.</param>
        public void SetParent(ActorBase newParent)
        {
            InternalSetParent(NativePtr, newParent? newParent.NativePtr : System.IntPtr.Zero);
        }
        
        /// <summary>
        ///     Adds given child to the children of this actor.
        /// </summary>
        /// <param name="child">The children instance.</param>
        public void AddChild(ActorBase child)
        {
            InternalAddChild(NativePtr, child? child.NativePtr : System.IntPtr.Zero);
        }
        
        /// <summary>
        ///     Removes given child from being child of this actor.
        /// </summary>
        /// <param name="child">The children instance.</param>
        public void RemoveChild(ActorBase child)
        {
            InternalRemoveChild(NativePtr, child? child.NativePtr : System.IntPtr.Zero);
        }
        
        /// <summary>
        ///     Adds given script to this actor.
        /// </summary>
        public void AddScript(Script script)
        {
            InternalAddScript(NativePtr, script? script.NativePtr : System.IntPtr.Zero);
        }
        
        /// <summary>
        ///     Removes given script to this actor.
        /// </summary>
        public void RemoveScript(Script script)
        {
            InternalRemoveScript(NativePtr, script? script.NativePtr : System.IntPtr.Zero);
        }
        
        /// <summary>
        ///     Sets active state.
        /// </summary>
        /// <param name="active">The active state.</param>
        public void SetActive(bool active)
        {
            InternalSetActive(NativePtr, active);
        }

        /// <summary>
        ///     Gets the parent of this actor.
        /// </summary>
        public ActorBase Parent
        {
            get
            {
                Get_InternalParent(NativePtr, out var data);
                return data;
            }
        }

        /// <summary>
        ///     Gets or sets world-space position of this actor.
        /// </summary>
        public virtual Vector3 Position
        {
            get
            {
                Get_InternalPosition(NativePtr, out var data);
                return data;
            }
            set => Set_InternalPosition(NativePtr, ref value);
        }

        /// <summary>
        ///     Gets or sets local-space position of this actor.
        ///     It is relative to the parent actor, if there is none,
        ///     this returns the same value as Position.
        /// </summary>
        public virtual Vector3 LocalPosition
        {
            get
            {
                Get_InternalLocalPosition(NativePtr, out var data);
                return data;
            }
            set => Set_InternalLocalPosition(NativePtr, ref value);
        }

        /// <summary>
        ///     Gets or sets world-space rotation of this actor.
        /// </summary>
        public virtual Quaternion Rotation
        {
            get
            {
                Get_InternalRotation(NativePtr, out var data);
                return data;
            }
            set => Set_InternalRotation(NativePtr, ref value);
        }

        /// <summary>
        ///     Gets or sets local-space rotation of this actor.
        ///     It is relative to the parent actor, if there is none,
        ///     this returns the same value as Rotation.
        /// </summary>
        public virtual Quaternion LocalRotation
        {
            get
            {
                Get_InternalLocalRotation(NativePtr, out var data);
                return data;
            }
            set => Set_InternalLocalRotation(NativePtr, ref value);
        }

        /// <summary>
        ///     Gets or sets world-space scale of this actor.
        /// </summary>
        public virtual Vector3 Scale
        {
            get
            {
                Get_InternalScale(NativePtr, out var data);
                return data;
            }
            set => Set_InternalScale(NativePtr, ref value);
        }

        /// <summary>
        ///     Gets or sets local-space scale of this actor.
        ///     It is relative to the parent actor, if there is none,
        ///     this returns the same value as Scale.
        /// </summary>
        public virtual Vector3 LocalScale
        {
            get
            {
                Get_InternalLocalScale(NativePtr, out var data);
                return data;
            }
            set => Set_InternalLocalScale(NativePtr, ref value);
        }

        /// <summary>
        ///     Gets or sets the transform of this actor.
        /// </summary>
        public virtual Transform Transform
        {
            get
            {
                Get_InternalTransform(NativePtr, out var data);
                return data;
            }
            set => Set_InternalTransform(NativePtr, ref value);
        }

        /// <summary>
        ///     Returns true when this actor's enable state is true.
        /// </summary>
        public bool IsActiveSelf
        {
            get
            {
                Get_InternalIsActiveSelf(NativePtr, out var data);
                return data;
            }
        }

        /// <summary>
        ///     Returns true when this actor's enable state is true and
        ///     false when parent's IsActive is also false.
        /// </summary>
        public bool IsActive
        {
            get
            {
                Get_InternalIsActive(NativePtr, out var data);
                return data;
            }
        }

        /// <summary>
        ///     Gets or sets the name of this actor.
        /// </summary>
        public string Name
        {
            get
            {
                Get_InternalName(NativePtr, out var data);
                return data;
            }
            set => Set_InternalName(NativePtr, ref value);
        }

        /// <summary>
        ///     Gets all children instances.
        /// </summary>
        public ActorBase[] Children
        {
            get
            {
                Get_InternalChildren(NativePtr, out var data);
                return data;
            }
        }

        /// <summary>
        ///     Gets all script instances contained by this actor.
        /// </summary>
        public Script[] Scripts
        {
            get
            {
                Get_InternalScripts(NativePtr, out var data);
                return data;
            }
        }

        /// <summary>
        ///     Gets the id of this actor.
        /// </summary>
        public ulong Id
        {
            get
            {
                Get_InternalId(NativePtr, out var data);
                return data;
            }
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetParent(System.IntPtr nativePtr, System.IntPtr newParent);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalAddChild(System.IntPtr nativePtr, System.IntPtr child);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalRemoveChild(System.IntPtr nativePtr, System.IntPtr child);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalAddScript(System.IntPtr nativePtr, System.IntPtr script);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalRemoveScript(System.IntPtr nativePtr, System.IntPtr script);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetActive(System.IntPtr nativePtr, bool active);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalParent(System.IntPtr nativePtr, out ActorBase data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalPosition(System.IntPtr nativePtr, out Vector3 data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalPosition(System.IntPtr nativePtr, ref Vector3 data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalLocalPosition(System.IntPtr nativePtr, out Vector3 data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalLocalPosition(System.IntPtr nativePtr, ref Vector3 data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalRotation(System.IntPtr nativePtr, out Quaternion data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalRotation(System.IntPtr nativePtr, ref Quaternion data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalLocalRotation(System.IntPtr nativePtr, out Quaternion data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalLocalRotation(System.IntPtr nativePtr, ref Quaternion data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalScale(System.IntPtr nativePtr, out Vector3 data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalScale(System.IntPtr nativePtr, ref Vector3 data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalLocalScale(System.IntPtr nativePtr, out Vector3 data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalLocalScale(System.IntPtr nativePtr, ref Vector3 data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalTransform(System.IntPtr nativePtr, out Transform data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalTransform(System.IntPtr nativePtr, ref Transform data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalIsActiveSelf(System.IntPtr nativePtr, out bool data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalIsActive(System.IntPtr nativePtr, out bool data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalName(System.IntPtr nativePtr, out string data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalName(System.IntPtr nativePtr, ref string data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalChildren(System.IntPtr nativePtr, out ActorBase[] data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalScripts(System.IntPtr nativePtr, out Script[] data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalId(System.IntPtr nativePtr, out ulong data);
    }
}