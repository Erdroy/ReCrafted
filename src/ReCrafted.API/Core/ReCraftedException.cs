// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

using System;

namespace ReCrafted.API.Core
{
    public class ReCraftedException : Exception
    {
        public ReCraftedException(string message) : base(message)
        {
        }
    }
}
