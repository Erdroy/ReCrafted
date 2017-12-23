// ReCrafted © 2016-2017 Always Too Late

using System.Collections.Generic;
using System.Linq;
using ReCrafted.API.Common;
using ReCrafted.API.Core;
using ReCrafted.API.Graphics;
using ReCrafted.API.Mathematics;
using ReCrafted.API.UI;

namespace ReCrafted.Game
{
    public class FreeCameraController : Script
    {
        private readonly List<Vector2> _deltaBuffer = new List<Vector2>();
        private Vector2 _lastDelta = Vector2.Zero;

        protected internal override void OnCreate()
        {
            Camera = Camera.Create();
            Camera.SetAsCurrent();
            Camera.Position = Entity.Position;
        }

        protected override void OnUpdate()
        {
            if (UIControl.FocusedControl != null)
                return;

            UpdateLook();
            UpdateMovement();

            // Update Camera position
            Camera.Position = Vector3.Lerp(Camera.Position, Entity.Position, CameraSmoothing * (float)Time.DeltaTime);
        }

        private void UpdateLook()
        {
            if(!Cursor.Lock)
                return;

            var cursorDelta = Input.CursorDelta;

            // Negate
            cursorDelta *= -Vector2.One;
            
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

            var rotation = Camera.Rotation + new Vector3(cursorDelta.X / 10.0f, cursorDelta.Y / 10.0f, 0.0f);
            rotation.Y = MathUtil.Clamp(rotation.Y, -89.9f, 89.9f);
            Camera.Rotation = rotation;
        }

        private void UpdateMovement()
        {
            var direction = Vector3.Zero;

            var currentSpeed = 20.0f;

            if (Input.IsKey(Keys.Shift))
                currentSpeed *= 2.5f;

            if (Input.IsKey(Keys.Control))
                currentSpeed *= 0.025f;

            if (Input.IsKey(Keys.W))
                direction += Camera.Forward;
            if (Input.IsKey(Keys.S))
                direction -= Camera.Forward;
            if (Input.IsKey(Keys.A))
                direction -= Camera.Right;
            if (Input.IsKey(Keys.D))
                direction += Camera.Right;
            if (Input.IsKey(Keys.E))
                direction += Camera.Up;
            if (Input.IsKey(Keys.Q))
                direction -= Camera.Up;

            direction.Normalize();
            direction *= currentSpeed;

            Entity.Position += direction * (float)Time.DeltaTime;
        }

        public bool MouseFiltering { get; set; } = true;

        public int MouseFilteringFrames { get; set; } = 3;

        public bool MouseAcceleration { get; set; } = true;

        public float MouseAccelerationMultiplier { get; set; } = 0.7f;

        public float MouseSensitivity { get; set; } = 2.0f;

        public Camera Camera { get; private set; }

        public float CameraSmoothing { get; set; } = 20.0f;
    }
}
