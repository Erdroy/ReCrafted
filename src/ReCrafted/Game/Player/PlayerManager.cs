// ReCrafted (c) 2016-2018 Always Too Late

using ReCrafted.API.Common;
using ReCrafted.API.Graphics;
using ReCrafted.API.Mathematics;
using ReCrafted.API.Physics;

namespace ReCrafted.Game.Player
{
    public class PlayerManager : Script
    {
        private Camera _camera;

        private void Start()
        {
            _camera = Camera.Create();
            _camera.SetAsCurrent();
        }

        public static PlayerManager SpawnPlayer(Vector3 position)
        {
            var character = CharacterActor.Create();
            character.Radius = 0.35f;
            character.Height = 1.75f;
            character.SlopeLimit = 45.0f;
            character.UpDirection = Vector3.Up;
            character.Position = position;

            Current = character.AddScript<PlayerManager>();
            Current.Movement = character.AddScript<PlayerMovement>();

            return Current;
        }

        public PlayerMovement Movement { get; private set; }

        public static PlayerManager Current { get; private set; }
    }
}
