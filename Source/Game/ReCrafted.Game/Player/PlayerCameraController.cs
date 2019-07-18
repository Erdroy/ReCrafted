// ReCrafted (c) 2016-2019 Always Too Late

using ReCrafted.API;
using ReCrafted.API.Mathematics;
using ReCrafted.API.Physics;
using ReCrafted.API.Rendering;

namespace ReCrafted.Game.Player
{
    /// <summary>
    ///     PlayerCameraController script.
    ///     Implements camera controlling behaviour and also position smoothing.
    /// </summary>
    public sealed class PlayerCameraController : Script<CharacterActor>
    {
        private Vector3 _poleDirection = Vector3.Right;

        private void Awake()
        {
            // Create camera
            Camera = New<CameraActor>();
            Camera.LocalPosition = new Vector3(0.0f, 0.7f, 0.0f);
            Actor.AddChild(Camera);
        }

        public void UpdateRotation(PlayerInput.Snapshot inputSnapshot)
        {
            var up = Vector3.Normalize(Actor.Position); // TODO: Use gravitational fields

            _poleDirection = Vector3.Normalize(_poleDirection - Vector3.Dot(up, _poleDirection) * up);

            var forward = Vector3.Cross(_poleDirection, up);
            var relativeRotation = Quaternion.LookRotation(forward, up);

            // Calculate final rotations
            Actor.UpDirection = up;
            Actor.Rotation = Quaternion.RotationAxis(up, MathUtil.DegreesToRadians(inputSnapshot.LookRotation.X)) * relativeRotation;
            Camera.LocalRotation = Quaternion.RotationAxis(Vector3.Right, MathUtil.DegreesToRadians(inputSnapshot.LookRotation.Y));
        }
        
        public CameraActor Camera { get; set; }
    }
}
