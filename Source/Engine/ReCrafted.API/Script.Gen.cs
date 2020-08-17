// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

using ReCrafted.API.Common;
using ReCrafted.API.Core;
using ReCrafted.API.Core.Actors;
using System.Runtime.CompilerServices;

namespace ReCrafted.API 
{
    public partial class Script : Object
    {

        /// <summary>
        ///     Gets or sets the script's enabled state,
        ///     When script is not enabled, it will not get Update/LateUpdate etc. calls.
        ///     When script is disabled or enabled, this will call it's OnEnable/OnDisable functions.
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
        private static extern ActorBase InternalActor(System.IntPtr nativePtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalEnabled(System.IntPtr nativePtr, out bool data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalEnabled(System.IntPtr nativePtr, ref bool data);
    }
}