// ReCrafted (c) 2016-2019 Always Too Late

using ReCrafted.API.Mathematics;

namespace ReCrafted.API.Physics
{
    public partial class CharacterActor
    {
        /// <summary>
        /// Moves the character actor along move vector.
        /// </summary>
        /// <param name="move">The move vector that represents current movement direction and length.</param>
        /// <returns>The collision flags.</returns>
        public CharacterCollisionFlags Move(Vector3 move)
        {
            return Internal_MoveCharacter(NativePtr, ref move);
        }

        /// <summary>
        /// Creates new actor of type 'CharacterActor'.
        /// </summary>
        /// <returns>The newly created actor.</returns>
        public static CharacterActor Create()
        {
            return New<CharacterActor>();
        }
    }
}
