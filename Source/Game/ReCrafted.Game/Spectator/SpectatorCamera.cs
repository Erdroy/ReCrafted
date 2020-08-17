// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

using System.Collections.Generic;
using System.Linq;
using ReCrafted.API;
using ReCrafted.API.Core;
using ReCrafted.API.Input;
using ReCrafted.API.Mathematics;
using ReCrafted.API.Rendering;

namespace ReCrafted.Game.Spectator
{
    public class SpectatorCamera : Script<CameraActor>
    {
        private readonly List<Vector2> _deltaBuffer = new List<Vector2>();
        private Vector2 _lastDelta = Vector2.Zero;
        private Vector3 _currentRotation = Vector3.Zero;

        private Vector3 _poleDirection = Vector3.Right;

        private float _currentSpeed = 2.0f;

        private void Update()
        {
            UpdateLook();
            UpdateMovement();
        }

        private void UpdateLook()
        {
            if (!InputManager.LockCursor)
                return;

            var cursorDelta = InputManager.CursorDelta * MouseSensitivity;

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

            var rotation = _currentRotation + new Vector3(cursorDelta.X / 10.0f, cursorDelta.Y / 10.0f, 0.0f);
            rotation.Y = MathUtil.Clamp(rotation.Y, -89.9f, 89.9f);
            _currentRotation = rotation;

            var up = Vector3.Normalize(Actor.Position); // TODO: Use gravitational fields

            _poleDirection = Vector3.Normalize(_poleDirection - Vector3.Dot(up, _poleDirection) * up);

            var forward = Vector3.Cross(_poleDirection, up);
            var relativeRotation = Quaternion.LookRotation(forward, up);

            // Calculate final rotations
            var cameraRotation = Quaternion.RotationAxis(up, MathUtil.DegreesToRadians(_currentRotation.X)) * 
                                 relativeRotation * 
                                 Quaternion.RotationAxis(Vector3.Right, MathUtil.DegreesToRadians(_currentRotation.Y));
            cameraRotation.Normalize();

            Actor.Rotation = cameraRotation;
        }

        private void UpdateMovement()
        {
            var direction = Vector3.Zero;

            // Add scroll controlled movement speed
            _currentSpeed += InputManager.ScrollDelta * 0.35f;
            _currentSpeed = MathUtil.Clamp(_currentSpeed, 0.5f, 10.0f);

            var currentSpeed = _currentSpeed * _currentSpeed;

            if (InputManager.IsKey(Key.Shift))
                currentSpeed *= 2.5f;

            if (InputManager.IsKey(Key.Control))
                currentSpeed *= 0.05f;

            if (InputManager.IsKey(Key.W))
                direction += Actor.Transform.Forward;
            if (InputManager.IsKey(Key.S))
                direction -= Actor.Transform.Forward;
            if (InputManager.IsKey(Key.A))
                direction -= Actor.Transform.Right;
            if (InputManager.IsKey(Key.D))
                direction += Actor.Transform.Right;
            if (InputManager.IsKey(Key.E))
                direction += Actor.Transform.Up;
            if (InputManager.IsKey(Key.Q))
                direction -= Actor.Transform.Up;

            direction.Normalize();
            direction *= currentSpeed;

            Actor.Position += direction * Time.DeltaTime;
        }

        public bool MouseFiltering { get; set; } = true;

        public int MouseFilteringFrames { get; set; } = 3;

        public bool MouseAcceleration { get; set; } = false;

        public float MouseAccelerationMultiplier { get; set; } = 0.7f;

        public float MouseSensitivity { get; set; } = 1.0f;
    }
}
