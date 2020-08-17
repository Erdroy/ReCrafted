// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

using ReCrafted.API.Mathematics;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Physics 
{
    /// <summary>
    ///     PhysicsScene class. Provides physics scene functionality and actor management.
    ///     After creation the scene is initially disabled, please set Enabled to true to allow simulation.
    /// </summary>
    public sealed partial class PhysicsScene : Object
    {
        
        /// <summary>
        ///     Attaches given actor to this scene.
        /// </summary>
        public void AttachActor(RigidBodyActor actor)
        {
            InternalAttachActor(NativePtr, actor? actor.NativePtr : System.IntPtr.Zero);
        }
        
        /// <summary>
        ///     Detaches given actor from the scene.
        /// </summary>
        public void DetachActor(RigidBodyActor actor)
        {
            InternalDetachActor(NativePtr, actor? actor.NativePtr : System.IntPtr.Zero);
        }

        /// <summary>
        ///     Gets or sets the scene's enable state. When true, the scene will be simulated.
        /// </summary>
        public bool Enabled
        {
            get
            {
                Get_InternalEnabled(NativePtr, out var data);
                return data;
            }
            set => Set_InternalEnabled(NativePtr, ref value);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalAttachActor(System.IntPtr nativePtr, System.IntPtr actor);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalDetachActor(System.IntPtr nativePtr, System.IntPtr actor);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalEnabled(System.IntPtr nativePtr, out bool data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalEnabled(System.IntPtr nativePtr, ref bool data);
    }
}