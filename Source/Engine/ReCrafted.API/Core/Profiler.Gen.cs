// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

using System.Runtime.CompilerServices;

namespace ReCrafted.API.Core 
{
    /// <summary>
    ///     Profiler class.
    ///     Provides thread CPU and Unmanaged/Manager memory profiling functionality.
    /// </summary>
    public static partial class Profiler
    {
        
        /// <summary>
        ///     Starts new CPU profile with given name.
        /// </summary>
        /// <returns>The current profile ID, use it to end the CPU profile.</returns>
        /// <remarks>To end this profile, use EndCPUProfile(...) function.</remarks>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static uint BeginCPUProfile(string name)
        {
            return InternalBeginCPUProfile(name);
        }
        
        /// <summary>
        ///     Ends CPU profile.
        /// </summary>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static void EndCPUProfile(uint profileId = 0)
        {
            InternalEndCPUProfile(profileId);
        }
        
        /// <summary>
        ///     Starts new CPU profile with given name.
        /// </summary>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static void BeginProfile(string name)
        {
            InternalBeginProfile(name);
        }
        
        /// <summary>
        ///     Ends current CPU profile.
        /// </summary>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static void EndProfile()
        {
            InternalEndProfile();
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint InternalBeginCPUProfile(string name);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalEndCPUProfile(uint profileId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalBeginProfile(string name);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalEndProfile();
    }
}