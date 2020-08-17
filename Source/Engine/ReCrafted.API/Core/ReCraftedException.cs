// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

using System;

namespace ReCrafted.API.Core
{
    [Serializable]
    public class ReCraftedException : Exception
    {
        public ReCraftedException() { }

        public ReCraftedException(string message) : base(message)
        {
        }
    }
}
