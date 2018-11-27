// ReCrafted (c) 2016-2018 Always Too Late

using System;
using ReCrafted.API.Common;
using ReCrafted.API.Mathematics;
using ReCrafted.API.Physics;

namespace ReCrafted.Game.Player
{
    /// <summary>
    /// PlayerMovement script. Provides player movement simulation.
    /// </summary>
    public sealed class PlayerMovement : GenericScript<CharacterActor>
    {
        [Serializable]
        public class MovementSettings
        {
            public float GroundAcceleration = 120.0f;
            public float AirAcceleration = 12.5f;
            public float MaxVelocityGround = 7.5f;
            public float MaxVelocityAir = 250.0f;
        }

        // We probably would like to make this configurable from asset
        public static readonly MovementSettings ProneSettings = new MovementSettings
        {

        };

        public static readonly MovementSettings CrouchSettings = new MovementSettings
        {

        };

        public static readonly MovementSettings WalkSettings = new MovementSettings
        {

        };

        public static readonly MovementSettings RunSettings = new MovementSettings
        {

        };

        public const float JumpForce = 6.5f;

        private Vector3 _moveVelocity;
        private Vector3 _velocity;

        /// <summary>
        /// Simulates this player's character using given input snapshot (<see cref="PlayerInput.Snapshot"/>).
        /// </summary>
        /// <param name="inputSnapshot">The input snapshot that will be used for simulating this player's character.</param>
        /// <remarks>
        /// Simulate uses current actor rotation.
        /// </remarks>
        public void Simulate(PlayerInput.Snapshot inputSnapshot)
        {
            var direction = GetMoveDirection(inputSnapshot);
            var settings = GetMoveSettings(inputSnapshot);

            if (IsGrounded)
            {
                _moveVelocity = MoveGround(settings, GroundFriction, direction, _moveVelocity);

                if (_moveVelocity.Length() < 0.05f)
                    _moveVelocity = Vector3.Zero;
                
                // Add jump force or gravity force
                if (inputSnapshot.Jump)
                {
                    _velocity += Vector3.Normalize(CurrentGravity) * JumpForce;
                }
                else
                {
                    _velocity = -CurrentGravity;
                }
            }
            else
            {
                _moveVelocity = MoveAir(settings, direction, _moveVelocity);
            }

            // Apply gravitation
            //_velocity -= CurrentGravity * (float)Time.FixedDeltaTime;

            // Move the player
            var collisionFlags = Actor.Move((_moveVelocity + _velocity) * (float)Time.FixedDeltaTime);

            // Check if we are going to hit something with our head
            if ((collisionFlags & CharacterCollisionFlags.Up) != 0)
            {
                if (_moveVelocity.Y > 0.0f)
                    _moveVelocity.Y = 0.0f;
            }
            
            // Build final state
            IsGrounded = (collisionFlags & CharacterCollisionFlags.Down) != 0;
        }
        
        private MovementSettings GetMoveSettings(PlayerInput.Snapshot input)
        {
            var isGoingForward = MathUtil.Clamp(input.Vertical, 0.0f, 1.0f) - Math.Abs(input.Horizontal) > 0.0f;

            switch (input.Standing)
            {
                case PlayerInput.Snapshot.StandingType.Normal:
                    return input.Run && isGoingForward ? RunSettings : WalkSettings;
                case PlayerInput.Snapshot.StandingType.Crouch:
                    return CrouchSettings;
                case PlayerInput.Snapshot.StandingType.Prone:
                    return ProneSettings;
                default:
                    throw new ArgumentOutOfRangeException();
            }
        }

        private Vector3 GetMoveDirection(PlayerInput.Snapshot input)
        {
            var direction = new Vector3(input.Horizontal, 0.0f, input.Vertical);
            direction.Normalize();
            direction = Actor.TransformDirection(direction);
            return direction;
        }

        private static Vector3 Accelerate(Vector3 direction, Vector3 currentVelocity, float accelerate, float maxVelocity)
        {
            var projectionVelocity = Vector3.Dot(currentVelocity, direction);
            var accelerationVelocity = accelerate * (float)Time.FixedDeltaTime;

            if (projectionVelocity + accelerationVelocity > maxVelocity)
                accelerationVelocity = maxVelocity - projectionVelocity;

            return currentVelocity + direction * accelerationVelocity;
        }
        
        private Vector3 MoveGround(MovementSettings settings, float friction, Vector3 direction, Vector3 currentVelocity)
        {
            // Apply Friction
            var speed = currentVelocity.Length();

            if (Math.Abs(speed) > float.Epsilon)
            {
                var drop = speed * friction * Time.FixedDeltaTime;
                currentVelocity *= (float)(Math.Max(speed - drop, 0) / speed); // Scale the velocity based on friction.
            }

            return Accelerate(direction, currentVelocity, settings.GroundAcceleration, settings.MaxVelocityGround);
        }

        private Vector3 MoveAir(MovementSettings settings, Vector3 direction, Vector3 currentVelocity)
        {
            return Accelerate(direction, currentVelocity, settings.AirAcceleration, settings.MaxVelocityAir);
        }

        public bool IsGrounded { get; private set; }
        public float GroundFriction => 15.0f;
        public Vector3 CurrentGravity => Vector3.Normalize(Actor.Position) * 9.81f; // TODO: Use gravitational fields
    }
}
