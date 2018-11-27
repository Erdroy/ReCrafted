// ReCrafted (c) 2016-2018 Always Too Late

using System;
using System.Collections.Generic;
using System.Linq;
using ReCrafted.API.Common;
using ReCrafted.API.Core;
using ReCrafted.API.Mathematics;

namespace ReCrafted.Game.Player
{
    /// <summary>
    /// PlayerInput script. Provides player input sampling (<see cref="Snapshot"/>).
    /// </summary>
    public sealed class PlayerInput : Script
    {
        /// <summary>
        /// Snapshot structure. Provides information which keys been pressed at given frame.
        /// </summary>
        public struct Snapshot
        {
            public enum StandingType
            {
                Normal,
                Crouch,
                Prone
            }
            
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
            /// The stand type input value.
            /// </summary>
            public StandingType Standing { get; set; }

            /// <summary>
            /// The local look rotation.
            /// This value is filtered, accelerated and inverted,
            /// depending on the current game settings.
            /// </summary>
            public Vector2 LookRotation { get; set; }
        }

        private readonly List<Vector2> _deltaBuffer = new List<Vector2>();
        private Vector2 _lastDelta = Vector2.Zero;
        private Vector2 _currentRotation = Vector2.Zero;

        private Snapshot.StandingType _currentStandingType = Snapshot.StandingType.Normal;
        
        /// <summary>
        /// Samples and returns current input snapshot.
        /// </summary>
        /// <returns>The sampled input snapshot (<see cref="Snapshot"/>).</returns>
        public Snapshot SampleInput()
        {
            var result = new Snapshot();

            // TODO: Use game settings to check what keys should be sampled for each axis

            // Sample vertical input
            if (Input.IsKey(Keys.W))
                result.Vertical += 1.0f;
            if (Input.IsKey(Keys.S))
                result.Vertical -= 1.0f;

            // Sample horizontal input
            if (Input.IsKey(Keys.A))
                result.Horizontal -= 1.0f;
            if (Input.IsKey(Keys.D))
                result.Horizontal += 1.0f;

            // Sample jump input
            result.Jump = Input.IsKeyDown(Keys.Space);

            // Sample standing
            UpdateStanding(ref result);

            // Sample look
            UpdateLook(ref result);

            // Done
            return result;
        }

        private void UpdateStanding(ref Snapshot snapshot)
        {
            if (Input.IsKeyDown(Keys.LeftControl))
            {
                _currentStandingType = Snapshot.StandingType.Crouch;
            }

            if (Input.IsKeyUp(Keys.LeftControl) && _currentStandingType == Snapshot.StandingType.Crouch)
            {
                _currentStandingType = Snapshot.StandingType.Normal;
            }

            if (Input.IsKeyDown(Keys.Z))
            {
                _currentStandingType = Snapshot.StandingType.Prone;
            }

            if (Input.IsKeyUp(Keys.Z) && _currentStandingType == Snapshot.StandingType.Prone)
            {
                _currentStandingType = Snapshot.StandingType.Normal;
            }

            snapshot.Standing = _currentStandingType;
        }

        private void UpdateLook(ref Snapshot snapshot)
        {
            var cursorDelta = Input.CursorDelta;

            if (MouseFiltering)
            {
                // Update buffer
                _deltaBuffer.Add(cursorDelta);

                while (_deltaBuffer.Count > MouseFilteringFrames)
                    _deltaBuffer.RemoveAt(0);

                cursorDelta = _deltaBuffer.Aggregate(Vector2.Zero, (current, delta) => current + delta);
                cursorDelta /= _deltaBuffer.Count;
            }

            if (MouseAcceleration)
            {
                var tmp = cursorDelta;
                cursorDelta = (cursorDelta + _lastDelta) * MouseAccelerationMultiplier;
                _lastDelta = tmp;
            }

            var rotation = _currentRotation + new Vector2(cursorDelta.X * MouseHorizontalSensitivity, cursorDelta.Y * MouseVerticalSensitivity) * 0.1f;
            rotation.Y = MathUtil.Clamp(rotation.Y, -89.9f, 89.9f);
            _currentRotation = rotation;

            // Set look rotation in current snapshot
            snapshot.LookRotation = rotation;
        }

        // TODO: Move to game settings class
        public bool MouseFiltering { get; set; } = true;
        public int MouseFilteringFrames { get; set; } = 3;
        public bool MouseAcceleration { get; set; } = true;
        public float MouseAccelerationMultiplier { get; set; } = 0.7f;
        public float MouseHorizontalSensitivity { get; set; } = 1.0f;
        public float MouseVerticalSensitivity { get; set; } = 1.0f;
    }
}
