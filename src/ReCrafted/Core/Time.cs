// ReCrafted © 2016 Damian 'Erdroy' Korczowski

using System;

namespace ReCrafted.Core
{
    /// <summary>
    /// Time class.
    /// </summary>
    public static class Time
    {
        /// <summary>
        /// The startup time.
        /// </summary>
        internal static DateTime StartupTime;

        /// <summary>
        /// The actual frame number.
        /// </summary>
        public static int Frame { get; internal set; }

        /// <summary>
        /// The actual simulation frame number.
        /// </summary>
        public static int SimulationFrame { get; internal set; }

        /// <summary>
        /// The frame delta time.
        /// </summary>
        public static float DeltaTime { get; internal set; }

        /// <summary>
        /// The smooth frame delta time.
        /// </summary>
        public static float SmoothDeltaTime { get; internal set; }

        /// <summary>
        /// The physics simulation delta time.
        /// </summary>
        public static float SimulationDeltaTime { get; set; }

        /// <summary>
        /// The frames per second count.
        /// </summary>
        public static float FramesPerSecond { get; internal set; } // TODO

        /// <summary>
        /// The smooth frames per second count.
        /// </summary>
        public static float SmoothFramesPerSecond { get; internal set; } // TODO

        /// <summary>
        /// Time since the startup time.
        /// </summary>
        public static float TimeSinceStartup { get; internal set; }

        /// <summary>
        /// The simulations per second count.
        /// </summary>
        public static float SimulationsPerSecond => 1.0f / SimulationDeltaTime;

        /// <summary>
        /// Realtime time since the startup.
        /// </summary>
        public static float RealtimeTimeSinceStartup => (float)(DateTime.Now - StartupTime).TotalSeconds;
    }
}