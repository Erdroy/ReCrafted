// ReCrafted (c) 2016-2018 Always Too Late

using System.Collections.Generic;
using System.Linq;
using ReCrafted.API.Common;
using ReCrafted.API.Core;
using ReCrafted.API.Graphics;
using ReCrafted.API.Mathematics;
using ReCrafted.API.Physics;

namespace ReCrafted.Game.Player
{
    public class PlayerMovement : Script
    {
        private readonly List<Vector2> _deltaBuffer = new List<Vector2>();
        private Vector2 _lastDelta = Vector2.Zero;
        private Vector3 _currentRotation = Vector3.Zero;

        private Vector3 _velocity;

        private void Update()
        {
            var actor = GetActor<CharacterActor>();

            var moveDirection = Vector3.Zero;
            if (Input.IsKey(Keys.W))
                moveDirection += Vector3.ForwardLH;
            if (Input.IsKey(Keys.S))
                moveDirection -= Vector3.ForwardLH;
            if (Input.IsKey(Keys.A))
                moveDirection -= Vector3.Right;
            if (Input.IsKey(Keys.D))
                moveDirection += Vector3.Right;
            moveDirection.Normalize();
            moveDirection = Actor.TransformDirection(moveDirection);

            // Calculate temporary gravity vector to planet's center
            var gravity = -actor.Position;
            gravity.Normalize();
            gravity *= 9.81f;
            actor.UpDirection = Vector3.Normalize(-gravity);

            if (actor.IsGrounded)
            {
                _velocity = moveDirection;

                if (Input.IsKey(Keys.Shift))
                {
                    _velocity *= 10.0f;
                }
                else
                {
                    _velocity *= 6.0f;
                }

                if (Input.IsKey(Keys.Space))
                {
                    _velocity += Vector3.Normalize(actor.Position) * 6.5f;
                }
            }
            else
            {
                _velocity += gravity * (float)Time.DeltaTime;
            }

            actor.Move(_velocity * (float)Time.DeltaTime);

            UpdateLook();
        }

        private void UpdateLook()
        {
            if (!Cursor.Lock)
                return;

            var character = GetActor<CharacterActor>();
            var camera = Actor.GetChild(0);

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

            var rotation = _currentRotation + new Vector3(cursorDelta.X / 10.0f, cursorDelta.Y / 10.0f, 0.0f);
            rotation.Y = MathUtil.Clamp(rotation.Y, -89.9f, 89.9f);
            _currentRotation = rotation;

            var up = Vector3.Normalize(character.Position);
            
            var v1 = Vector3.Normalize(up);
            var v2 = Vector3.Normalize(Vector3.Cross(Vector3.ForwardLH, up));
            var v3 = Vector3.Normalize(Vector3.Cross(v1, v2));
            
            // TODO: Fix (forward) pole issue

            var characterRotation = Quaternion.RotationAxis(v1, MathUtil.DegreesToRadians(_currentRotation.X));
            character.Rotation = characterRotation * Quaternion.LookRotation(v3, v1);
            
            var cameraRotation = Quaternion.RotationAxis(Vector3.Right, MathUtil.DegreesToRadians(_currentRotation.Y));
            camera.LocalRotation = cameraRotation;
        }
        
        public bool MouseFiltering { get; set; } = true;

        public int MouseFilteringFrames { get; set; } = 3;

        public bool MouseAcceleration { get; set; } = true;

        public float MouseAccelerationMultiplier { get; set; } = 0.7f;
    }
}
