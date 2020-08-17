// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

using System.Runtime.CompilerServices;

namespace ReCrafted.API.Core 
{
    /// <summary>
    ///     The time class. Provides engine's timing services.
    /// </summary>
    public static partial class Time
    {

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float InternalCurrentTime();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float InternalCurrentFixedTime();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float InternalDeltaTime();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float InternalFixedDeltaTime();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float InternalFrameAlpha();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern double InternalRealtime();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint InternalCurrentFrame();
    }
}