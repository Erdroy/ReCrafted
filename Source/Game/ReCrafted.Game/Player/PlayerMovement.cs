// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using System;
using ReCrafted.API;
using ReCrafted.API.Core;
using ReCrafted.API.Mathematics;
using ReCrafted.API.Physics;
using ReCrafted.API.Rendering;

namespace ReCrafted.Game.Player
{
    /// <summary>
    ///     PlayerMovement script. Provides player movement simulation.
    /// </summary>
    public sealed class PlayerMovement : Script<CharacterActor>
    {
        [Serializable]
        public class MovementSettings
        {
            public float GroundAcceleration = 50.0f;
            public float AirAcceleration = 5.0f;
            public float MaxVelocityGround = 7.0f;
            public float MaxVelocityAir = 5.0f;
        }

        // We probably would like to make this configurable from asset
        public static readonly MovementSettings ProneSettings = new MovementSettings
        {
            GroundAcceleration = 40.0f,
            MaxVelocityGround = 2.0f
        };

        public static readonly MovementSettings CrouchSettings = new MovementSettings
        {
            GroundAcceleration = 40.0f,
            MaxVelocityGround = 3.5f
        };

        public static readonly MovementSettings WalkSettings = new MovementSettings
        {
            GroundAcceleration = 50.0f,
            MaxVelocityGround = 6.0f
        };

        public static readonly MovementSettings RunSettings = new MovementSettings
        {
            GroundAcceleration = 60.0f,
            MaxVelocityGround = 10.0f
        };

        public const float JumpForce = 7.5f;

        private Vector3 _velocity;
        private CameraActor _camera;

        private void Start()
        {
            _camera = Actor.Children[0] as CameraActor;
        }

        /// <summary>
        ///     Simulates this player's character using given input snapshot (<see cref="PlayerInput.Snapshot"/>).
        /// </summary>
        /// <param name="input">The input snapshot that will be used for simulating this player's character.</param>
        /// <remarks>
        /// Simulate uses current actor rotation.
        /// </remarks>
        public void SimulateMovement(PlayerInput.Snapshot input)
        {
            var direction = GetMoveDirection(input);
            var settings = GetMoveSettings(input);

            switch (input.Movement)
            {
                case PlayerInput.Snapshot.MovementType.Normal:
                    DoNormalMovement(input, direction, settings);
                    break;
                case PlayerInput.Snapshot.MovementType.JetPack:
                    DoJetPackMovement(input, direction, settings);
                    break;
                default:
                    throw new ArgumentOutOfRangeException();
            }
        }

        private void DoJetPackMovement(PlayerInput.Snapshot input, Vector3 direction, MovementSettings settings)
        {
            // TODO: Apply roll
            //Actor.Rotation *= Quaternion.RotationAxis(_camera.Transform.Forward, input.Roll * Time.DeltaTime * 5.0f);
            Actor.UpDirection = Actor.Transform.Up;

            _velocity += direction * Time.DeltaTime * 20.0f;

            // Apply velocity damping
            if (input.JetPackDamping && _velocity.LengthSquared() > 0.1f)
                _velocity -= _velocity * Time.DeltaTime * 2.0f;

            Actor.Move(_velocity * Time.DeltaTime);
        }

        private void DoNormalMovement(PlayerInput.Snapshot input, Vector3 direction, MovementSettings settings)
        {
            if (Actor.IsGrounded)
            {
                _velocity = MoveGround(settings, 8.0f, direction, _velocity);
                
                if (input.Jump)
                    _velocity += Vector3.Normalize(CurrentGravity) * JumpForce;

                _velocity -= CurrentGravity * Time.DeltaTime;
            }
            else
            {
                _velocity = MoveAir(settings, direction, _velocity);
            }

            _velocity -= CurrentGravity * Time.DeltaTime;

            Actor.Move(_velocity * Time.DeltaTime);
        }

        /*private Vector3 GetSurfaceNormal()
        {
            return PhysicsManager.RayCast(Actor.Position, -Actor.UpDirection, out var hit, 2.0f, 1) 
                ? hit.Normal : Actor.UpDirection;
        }

        private float GetSurfaceSlope()
        {
            return Vector3.Angle(Actor.UpDirection, GetSurfaceNormal());
        }*/

        private MovementSettings GetMoveSettings(PlayerInput.Snapshot input)
        {
            var isGoingForward = MathUtil.Clamp(input.Vertical, 0.0f, 1.0f) - Math.Abs(input.Horizontal) > 0.0f;

            switch (input.Standing)
            {
                case PlayerInput.Snapshot.StandingType.Normal:
                    return input.Run ? RunSettings : WalkSettings;
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
            var direction = new Vector3(input.Horizontal, input.Movement == PlayerInput.Snapshot.MovementType.JetPack ? input.Thrust : 0.0f, input.Vertical);
            direction.Normalize();

            if (input.Movement == PlayerInput.Snapshot.MovementType.Normal)
                direction = Actor.TransformDirection(direction);
            else
                direction = _camera.TransformDirection(direction);

            return direction;
        }

        private static Vector3 Accelerate(Vector3 direction, Vector3 currentVelocity, float accelerate, float maxVelocity)
        {
            var projectionVelocity = Vector3.Dot(currentVelocity, direction);
            var accelerationVelocity = accelerate * Time.DeltaTime;

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
                var drop = speed * friction * Time.DeltaTime;
                currentVelocity *= Math.Max(speed - drop, 0) / speed; // Scale the velocity based on friction.
            }

            return Accelerate(direction, currentVelocity, settings.GroundAcceleration, settings.MaxVelocityGround);
        }

        private Vector3 MoveAir(MovementSettings settings, Vector3 direction, Vector3 currentVelocity)
        {
            return Accelerate(direction, currentVelocity, settings.AirAcceleration, settings.MaxVelocityAir);
        }

        public bool IsGrounded => Actor.IsGrounded;
        public Vector3 CurrentGravity => Vector3.Normalize(Actor.Position) * 9.81f; // TODO: Use gravitational fields
    }
}
