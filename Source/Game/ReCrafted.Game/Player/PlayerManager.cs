// ReCrafted (c) 2016-2019 Always Too Late

using ReCrafted.API;
using ReCrafted.API.Mathematics;
using ReCrafted.API.Physics;

namespace ReCrafted.Game.Player
{
    /// <summary>
    ///     PlayerManager class.
    /// </summary>
    public sealed class PlayerManager : Script<CharacterActor>
    {
        /// <summary>
        ///     The character controller radius.
        /// </summary>
        public const float PlayerRadius = 0.35f;

        /// <summary>
        ///     The character controller height.
        /// </summary>
        public const float PlayerHeight = 1.75f;

        private void Awake()
        {
            // Setup all needed scripts
            Input = Actor.AddScript<PlayerInput>();
            CameraController = Actor.AddScript<PlayerCameraController>();
            Movement = Actor.AddScript<PlayerMovement>();

            IsOwner = true;
        }

        private void Update()
        {
            if (!IsOwner)
                return;

            // Sample player input
            var snapshot = Input.SampleInput();

            // Update player camera rotation
            CameraController.UpdateRotation(snapshot);

            // Simulate player movement
            Movement.SimulateMovement(snapshot);
        }
        
        /// <summary>
        ///     Spawns player at given position with given rotation.
        /// </summary>
        /// <param name="position">The player position.</param>
        /// <param name="rotation">The player rotation.</param>
        /// <returns>The spawned player's manager script instance.</returns>
        public static PlayerManager SpawnPlayer(Vector3 position, Quaternion rotation)
        {
            // Setup character actor
            var character = New<CharacterActor>();
            character.Radius = PlayerRadius;
            character.Height = PlayerHeight;
            character.UpDirection = Vector3.Up;
            character.Position = position;

            // Add PlayerManager and return it
            return character.AddScript<PlayerManager>();
        }
        
        /// <summary>
        ///     <see cref="PlayerMovement"/> instance owned by this player.
        /// </summary>
        public PlayerMovement Movement { get; private set; }

        /// <summary>
        ///     <see cref="PlayerInput"/> instance owned by this player.
        /// </summary>
        public PlayerInput Input { get; private set; }

        /// <summary>
        ///     <see cref="PlayerCameraController"/> instance owned by this player.
        /// </summary>
        public PlayerCameraController CameraController { get; private set; }

        /// <summary>
        ///     <see cref="PlayerViewController"/> instance owned by this player.
        /// </summary>
        public PlayerViewController ViewController { get; private set; }

        /// <summary>
        ///     True when the current client is owner of this player.
        /// </summary>
        public bool IsOwner { get; private set; }
    }
}
