// ReCrafted (c) 2016-2018 Always Too Late

using System.Collections.Generic;
using System.Linq;
using ReCrafted.API.Common;
using ReCrafted.API.Core;
using ReCrafted.API.Graphics;
using ReCrafted.API.Mathematics;
using ReCrafted.API.UI;

namespace ReCrafted.Game
{
    public class FreeCameraController
    {
        private readonly List<Vector2> _deltaBuffer = new List<Vector2>();
        private Vector2 _lastDelta = Vector2.Zero;
        private Vector3 _currentRotation = Vector3.Zero;

        public void Initialize()
        {
            Camera = Camera.Create();
            Camera.SetAsCurrent();
            Camera.Position = Position;
            Camera.FarPlane = 10000.0f;
        }

        public void Update()
        {
            if (UIControl.FocusedControl != null)
                return;

            UpdateLook();
            UpdateMovement();

            // Update Camera position
            Camera.Position = Vector3.Lerp(Camera.Position, Position, CameraSmoothing * (float)Time.DeltaTime);
        }

        private void UpdateLook()
        {
            if(!Cursor.Lock)
                return;

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

            var cameraRotation = Quaternion.RotationAxis(Vector3.Up, MathUtil.DegreesToRadians(_currentRotation.X)) *
                                 Quaternion.RotationAxis(Vector3.Right, MathUtil.DegreesToRadians(_currentRotation.Y));

            cameraRotation.Normalize();

            var up = Vector3.Normalize(Camera.Position);
            var forward = Vector3.Cross(Camera.Right, up);
            var right = Vector3.Cross(up, forward);
            
            DebugDraw.Color = Color.Red;
            DebugDraw.DrawArrow(Camera.Position + Camera.Forward * 4.0f, Camera.Position + up + Camera.Forward * 4.0f, 0.25f);
            DebugDraw.Color = Color.Green;
            DebugDraw.DrawArrow(Camera.Position + Camera.Forward * 4.0f, Camera.Position + forward + Camera.Forward * 4.0f, 0.25f);
            DebugDraw.Color = Color.Blue;
            DebugDraw.DrawArrow(Camera.Position + Camera.Forward * 4.0f, Camera.Position + right + Camera.Forward * 4.0f, 0.25f);

           // DebugDraw.Color = Color.Yellow;
            //DebugDraw.DrawArrow(Camera.Position + Camera.Forward * 4.0f, Camera.Position + lookAt.Axis + Camera.Forward * 4.0f, 0.25f);

            // TODO: Make cameraRotation relative to up vector

            Camera.Rotation = cameraRotation;
        }

        private void UpdateMovement()
        {
            var direction = Vector3.Zero;

            var currentSpeed = 50.0f;

            if (Input.IsKey(Keys.Shift))
                currentSpeed *= 2.5f;

            if (Input.IsKey(Keys.Control))
                currentSpeed *= 0.05f;

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

            Position += direction * (float)Time.DeltaTime;
        }

        public Vector3 Position { get; set; }

        public bool MouseFiltering { get; set; } = true;

        public int MouseFilteringFrames { get; set; } = 3;

        public bool MouseAcceleration { get; set; } = true;

        public float MouseAccelerationMultiplier { get; set; } = 0.7f;

        public float MouseSensitivity { get; set; } = 2.0f;

        public Camera Camera { get; private set; }

        public float CameraSmoothing { get; set; } = 20.0f;
    }
}
