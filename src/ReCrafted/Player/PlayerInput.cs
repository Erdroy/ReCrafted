// ReCrafted (c) 2016-2019 Always Too Late

using System.Collections.Generic;
using System.Linq;
using ReCrafted.API.Common;
using ReCrafted.API.Core;
using ReCrafted.API.Input;
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

            public enum MovementType
            {
                Normal,
                JetPack
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
            /// The run input value.
            /// When true, the player will try to run.
            /// </summary>
            public bool Run { get; set; }

            /// <summary>
            /// The jet pack damping input value.
            /// When true, the player jet pack will try to stop.
            /// </summary>
            public bool JetPackDamping { get; set; }

            /// <summary>
            /// The stand type input value.
            /// </summary>
            public StandingType Standing { get; set; }

            /// <summary>
            /// The movement type input value.
            /// </summary>
            public MovementType Movement { get; set; }

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
            if (InputManager.IsKey(Key.W))
                result.Vertical += 1.0f;
            if (InputManager.IsKey(Key.S))
                result.Vertical -= 1.0f;

            // Sample horizontal input
            if (InputManager.IsKey(Key.A))
                result.Horizontal -= 1.0f;
            if (InputManager.IsKey(Key.D))
                result.Horizontal += 1.0f;

            // Sample jump input
            result.Jump = InputManager.IsKeyDown(Key.Space);

            // Sample run input
            result.Run = InputManager.IsKey(Key.Shift);

            result.Movement = Snapshot.MovementType.Normal; // TODO: Movement type switch
            result.JetPackDamping = true;

            // Sample standing
            UpdateStanding(ref result);

            // Sample look
            UpdateLook(ref result);

            // Done
            return result;
        }

        private void UpdateStanding(ref Snapshot snapshot)
        {
            if (InputManager.IsKey(Key.Control))
            {
                _currentStandingType = Snapshot.StandingType.Crouch;
            }
            else if (InputManager.IsKey(Key.Z))
            {
                _currentStandingType = Snapshot.StandingType.Prone;
            }
            else
            {
                _currentStandingType = Snapshot.StandingType.Normal;
            }

            snapshot.Standing = _currentStandingType;
        }

        private void UpdateLook(ref Snapshot snapshot)
        {
            var cursorDelta = InputManager.CursorDelta;

            if (!InputManager.LockCursor)
                cursorDelta = Vector2.Zero;
            
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

            var rotation = _currentRotation + new Vector2(
                               MathUtil.DegreesToRadians(cursorDelta.X) * MouseHorizontalSensitivity,
                               MathUtil.DegreesToRadians(cursorDelta.Y) * MouseVerticalSensitivity
                           );
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
        public float MouseHorizontalSensitivity { get; set; } = 3.0f;
        public float MouseVerticalSensitivity { get; set; } = 3.0f;
    }
}
