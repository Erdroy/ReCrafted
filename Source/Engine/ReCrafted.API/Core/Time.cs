// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

namespace ReCrafted.API.Core
{
    /// <summary>
    ///     The time class. Provides engine's timing services.
    /// </summary>
    public sealed partial class Time
    {
        /// <summary>
        ///     Returns current frame engine's time.
        /// </summary>
        public static float CurrentTime => InternalCurrentTime();

        /// <summary>
        ///     Returns current frame fixed engine's time.
        /// </summary>
        public static float CurrentFixedTime => InternalCurrentFixedTime();

        /// <summary>
        ///     Returns the time between last frame and current one.
        /// </summary>
        public static float DeltaTime => InternalDeltaTime();

        /// <summary>
        ///     Returns the fixed time. This time is being used as time step
        ///     for the FixedUpdate function calls.
        /// </summary>
        public static float FixedDeltaTime => InternalFixedDeltaTime();

        /// <summary>
        ///     The current frame number.
        /// </summary>
        public static uint Frame => InternalCurrentFrame();
    }
}
