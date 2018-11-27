// ReCrafted (c) 2016-2018 Always Too Late

using System;
using ReCrafted.API.Common;

namespace ReCrafted.Game.Player
{
    /// <summary>
    /// PlayerInput script. Provides player input sampling (<see cref="Snapshot"/>).
    /// </summary>
    public class PlayerInput : Script
    {
        /// <summary>
        /// Snapshot structure. Provides information which keys been pressed at given frame.
        /// </summary>
        public struct Snapshot
        {
            /// <summary>
            /// The horizontal movement value (left-right).
            /// This value is not being filtered.
            /// </summary>
            public float Horizontal { get; set; }

            /// <summary>
            /// The vertical movement value (forward-backward).
            /// This value is not being filtered.
            /// </summary>
            public float Vertical { get; set; }

            /// <summary>
            /// The jump input value.
            /// When true, the player will try to jump.
            /// </summary>
            public bool Jump { get; set; }

            /// <summary>
            /// The crouch input value.
            /// When true, the player will try to crouch.
            /// </summary>
            public bool Crouch { get; set; }

            /// <summary>
            /// The prone input value.
            /// When true, the player will try to prone.
            /// </summary>
            public bool Prone { get; set; }
        }

        /// <summary>
        /// Samples and returns current input snapshot.
        /// </summary>
        /// <returns>The sampled input snapshot (<see cref="Snapshot"/>).</returns>
        public Snapshot SampleInput()
        {
            throw new NotImplementedException();
        }
    }
}
