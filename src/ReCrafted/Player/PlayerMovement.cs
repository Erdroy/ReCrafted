// ReCrafted (c) 2016-2018 Always Too Late

using System;
using ReCrafted.API.Common;
using ReCrafted.API.Physics;

namespace ReCrafted.Game.Player
{
    /// <summary>
    /// PlayerMovement script. Provides player movement simulation.
    /// </summary>
    public class PlayerMovement : GenericScript<CharacterActor>
    {
        /// <summary>
        /// Simulates this player's character using given input snapshot (<see cref="PlayerInput.Snapshot"/>).
        /// </summary>
        /// <param name="inputSnapshot">The input snapshot that will be used for simulating this player's character.</param>
        /// <remarks>
        /// Simulate uses current actor rotation.
        /// </remarks>
        public void Simulate(PlayerInput.Snapshot inputSnapshot)
        {
            throw new NotImplementedException();
        }
    }
}
