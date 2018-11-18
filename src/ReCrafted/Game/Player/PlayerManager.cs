// ReCrafted (c) 2016-2018 Always Too Late

using ReCrafted.API.Common;
using ReCrafted.API.Graphics;
using ReCrafted.API.Mathematics;
using ReCrafted.API.Physics;

namespace ReCrafted.Game.Player
{
    public class PlayerManager : Script
    {
        public static PlayerManager SpawnPlayer(Vector3 position)
        {
            var character = CharacterActor.Create();
            character.Radius = 0.35f;
            character.Height = 1.75f;
            character.SlopeLimit = 45.0f;
            character.UpDirection = Vector3.Up;
            character.Position = position;

            var player = Current = character.AddScript<PlayerManager>();
            Current.Movement = character.AddScript<PlayerMovement>();

            player.CameraActor = CameraActor.Create();
            player.CameraActor.LocalPosition = new Vector3(0.0f, 0.7f, 0.0f);
            character.AddChild(player.CameraActor);

            return Current;
        }

        public CameraActor CameraActor { get; private set; }

        public PlayerMovement Movement { get; private set; }

        public static PlayerManager Current { get; private set; }
    }
}
