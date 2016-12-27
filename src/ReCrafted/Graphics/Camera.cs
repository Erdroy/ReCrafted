// ReCrafted © 2016 Damian 'Erdroy' Korczowski

using System;
using System.Windows.Forms;
using ReCrafted.Core;
using SharpDX;

namespace ReCrafted.Graphics
{
    /// <summary>
    /// Camera class.
    /// </summary>
    public sealed class Camera : ITickable
    {
        /// <summary>
        /// The current camera which is used to render everything.
        /// </summary>
        public static Camera Current;

        private readonly Vector3[] _filtering = new Vector3[3];
        private int _filteringStep;

        private float _aspectRatio;
        private Vector3 _rotation;
        private Vector3 _lastDelta;

        /// <summary>
        /// Camera contstructor
        /// </summary>
        public Camera()
        {
            BackgroundColor = new Color(0.3f, 0.3f, 0.3f, 1.0f);

            NearZPlane = 0.02f;
            FarZPlane = 1000.0f;
            FieldOfView = 0.75f;

            LookAcceleration = true;
            LookFiltering = true;
            UseLook = true;

            Forward = Vector3.ForwardLH;
        }

        /// <summary>
        /// Sets this camera as `current` for rendering.
        /// </summary>
        public void SetAsCurrent()
        {
            Current = this;

            if (FreeLook)
            {
                // zeroe the cursor position
                var clientCenter = Game.Instance.Form.PointToScreen(new System.Drawing.Point(Game.Instance.Form.ClientSize.Width / 2, Game.Instance.Form.ClientSize.Height / 2));
                Cursor.Position = clientCenter;
            }
        }

        /// <summary>
        /// Ticks the camera, this is called by the game loop, 
        /// no need to call it again manually.
        /// </summary>
        public void Tick()
        {
            // check if this camera is active
            if (IsActive)
                return;

            if (FreeLook)
            {
                // update
                UpdateMovement();

                if (UseLook && Input.IsButton(ButtonCode.Right))
                {
                    UpdateLook();

                    Cursor.Position = Game.Instance.Form.PointToScreen(
                        new System.Drawing.Point(Game.Instance.Form.ClientSize.Width/2,
                            Game.Instance.Form.ClientSize.Height/2));
                    Cursor.Hide();
                }
                else
                {
                    Cursor.Show();
                }
            }

            UpdateMatrices();
        }

        /// <summary>
        /// Updates the view/projection matrices.
        /// </summary>
        public void UpdateMatrices()
        {
            _aspectRatio = Display.ClientWidth / (float)Display.ClientHeight;

            ViewMatrix = Matrix.LookAtLH(Position, Position + Forward, Vector3.Up);
            ProjectionMatrix = Matrix.PerspectiveFovLH(FieldOfView, _aspectRatio, NearZPlane, FarZPlane);
            
            CameraFrustum = new BoundingFrustum(ViewProjectionMatrix);
        }

        /// <summary>
        /// Calculates rotation matrix.
        /// </summary>
        /// <returns>The rotation matrix.</returns>
        public Matrix GetRotationMatrix()
        {
            var rot = CreateFromYawPitchRoll(MathUtil.DegreesToRadians(-_rotation.Y), MathUtil.DegreesToRadians(-_rotation.X), 0.0f);
            var rotMatrix = Matrix.RotationQuaternion(rot);

            return rotMatrix;
        }

        // private
        private void UpdateMovement()
        {
            var rotMatrix = GetRotationMatrix();

            var dir = Vector3.Zero;

            if (Input.IsKey(KeyCode.W))
                dir += rotMatrix.Forward;

            if (Input.IsKey(KeyCode.S))
                dir -= rotMatrix.Forward;

            if (Input.IsKey(KeyCode.A))
                dir += rotMatrix.Left;

            if (Input.IsKey(KeyCode.D))
                dir -= rotMatrix.Left;

            if (Input.IsKey(KeyCode.Q))
                dir += rotMatrix.Up;

            if (Input.IsKey(KeyCode.E))
                dir -= rotMatrix.Up;

            dir.Normalize();

            // speed
            if (Input.IsKey(KeyCode.ShiftKey))
                dir *= 15.0f;
            else if (Input.IsKey(KeyCode.ControlKey))
                dir *= 1.0f;
            else
                dir *= 6.0f;

            Position += dir * Time.DeltaTime;
        }

        // private
        private void UpdateLook()
        {
            var cursorPos = Cursor.Position;
            var clientCenter = Game.Instance.Form.PointToScreen(new System.Drawing.Point(Game.Instance.Form.ClientSize.Width / 2, Game.Instance.Form.ClientSize.Height / 2));
            var delta = new System.Drawing.Point(cursorPos.X - clientCenter.X, cursorPos.Y - clientCenter.Y);

            var mouseDelta = new Vector3(delta.X, delta.Y, 0.0f);

            if (LookFiltering)
            {
                // Filtering
                _filtering[_filteringStep] = new Vector3(delta.X, delta.Y, 0.0f);
                _filteringStep++;

                if (_filteringStep == 3)
                    _filteringStep = 0;

                // Calculate avg. mouse delta
                mouseDelta = (_filtering[0] + _filtering[1] + _filtering[2]) / new Vector3(3.0f, 3.0f, 1.0f);
            }

            // acceleration
            var accel = mouseDelta;

            if (LookAcceleration)
                accel += _lastDelta;

            _rotation += new Vector3(accel.X / 16.0f, accel.Y / 16.0f, 0.0f);
            _rotation.Y = MathUtil.Clamp(_rotation.Y, -89.0f, 89.0f);

            var rotMatrix = GetRotationMatrix();
            Forward = rotMatrix.Forward;

            _lastDelta = mouseDelta;

            Cursor.Position = clientCenter;
        }

        // private
        private static Quaternion CreateFromYawPitchRoll(float yaw, float pitch, float roll)
        {
            var rollOver2 = roll * 0.5f;
            var sinRollOver2 = (float)Math.Sin(rollOver2);
            var cosRollOver2 = (float)Math.Cos(rollOver2);
            var pitchOver2 = pitch * 0.5f;
            var sinPitchOver2 = (float)Math.Sin(pitchOver2);
            var cosPitchOver2 = (float)Math.Cos(pitchOver2);
            var yawOver2 = yaw * 0.5f;
            var sinYawOver2 = (float)Math.Sin(yawOver2);
            var cosYawOver2 = (float)Math.Cos(yawOver2);

            Quaternion result;
            result.X = cosYawOver2 * cosPitchOver2 * cosRollOver2 + sinYawOver2 * sinPitchOver2 * sinRollOver2;
            result.Y = cosYawOver2 * cosPitchOver2 * sinRollOver2 - sinYawOver2 * sinPitchOver2 * cosRollOver2;
            result.Z = cosYawOver2 * sinPitchOver2 * cosRollOver2 + sinYawOver2 * cosPitchOver2 * sinRollOver2;
            result.W = sinYawOver2 * cosPitchOver2 * cosRollOver2 - cosYawOver2 * sinPitchOver2 * sinRollOver2;
            return result;
        }

        // properties
        // camera info
        /// <summary>
        /// The background color, which will be used 
        /// as `clearing` color for render targets.
        /// </summary>
        public Color BackgroundColor { get; set; }

        /// <summary>
        /// Make the look acceleration enabled or disabled.
        /// </summary>
        public bool LookAcceleration { get; set; }

        /// <summary>
        /// Make the look filtering enabled or disabled.
        /// </summary>
        public bool LookFiltering { get; set; }

        /// <summary>
        /// Can camera use look at the moment?
        /// Default: true.
        /// </summary>
        public bool UseLook { get; set; }

        /// <summary>
        /// The far z plane.
        /// </summary>
        public float FarZPlane { get; set; }

        /// <summary>
        /// The near z plane.
        /// </summary>
        public float NearZPlane { get; set; }

        /// <summary>
        /// The field of view.
        /// </summary>
        public float FieldOfView { get; set; }

        /// <summary>
        /// Let this camera have a free look?
        /// </summary>
        public bool FreeLook { get; set; }

        // transform
        /// <summary>
        /// Camera position.
        /// </summary>
        public Vector3 Position { get; set; }

        /// <summary>
        /// The look/forward direction.
        /// </summary>
        public Vector3 Forward { get; set; }
        
        /// <summary>
        /// The down direction.
        /// </summary>
        public Vector3 Down => ViewMatrix.Down;

        /// <summary>
        /// The backward direction.
        /// </summary>
        public Vector3 Backward => ViewMatrix.Backward;

        /// <summary>
        /// The left direction.
        /// </summary>
        public Vector3 Left => ViewMatrix.Left;

        /// <summary>
        /// The right direction.
        /// </summary>
        public Vector3 Right => ViewMatrix.Right;

        /// <summary>
        /// True when this camera is the
        /// current rendering target camera.
        /// </summary>
        public bool IsActive => Current != this;

        /// <summary>
        /// Camera's frustum.
        /// </summary>
        public BoundingFrustum CameraFrustum { get; private set; }

        // matrices
        /// <summary>
        /// The View matrix.
        /// </summary>
        public Matrix ViewMatrix { get; private set; }

        /// <summary>
        /// The Projection matrix.
        /// </summary>
        public Matrix ProjectionMatrix { get; private set; }

        /// <summary>
        /// The View*Projection matrix.
        /// </summary>
        public Matrix ViewProjectionMatrix => ViewMatrix*ProjectionMatrix;
    }
}