﻿// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

namespace ReCrafted.API.Physics
{
    public enum CharacterCollisionFlags
    {
        None = 0,
        Sides = 1 << 0,
        Up = 1 << 1,
        Down = 1 << 2
    }
}
