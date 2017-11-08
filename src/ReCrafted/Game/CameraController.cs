// ReCrafted © 2016-2017 Always Too Late

using ReCrafted.API.Common;
using ReCrafted.API.Core;
using ReCrafted.API.Graphics;
using ReCrafted.API.Mathematics;

namespace ReCrafted.Game
{
    public class CameraController : Script
    {
        protected internal override void OnCreate()
        {
            Camera = Camera.Create();
            Camera.SetAsCurrent();
            Camera.Position = Entity.Position;
        }

        protected override void OnUpdate()
        {
            UpdateMovement();

            // Update Camera position
            Camera.Position = Vector3.Lerp(Camera.Position, Entity.Position, CameraSmoothing * (float)Time.DeltaTime);
        }

        private void UpdateMovement()
        {
            var direction = Vector3.Zero;

            var currentSpeed = 4.0f;

            if (Input.IsKey(Keys.Shift))
                currentSpeed *= 2.5f;

            if (Input.IsKey(Keys.Control))
                currentSpeed *= 0.25f;

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

        public Camera Camera { get; private set; }

        public float CameraSmoothing { get; set; } = 20.0f;

        public static CameraController Current { get; set; }
    }
}
