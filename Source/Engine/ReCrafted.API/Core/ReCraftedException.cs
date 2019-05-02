// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using System;

namespace ReCrafted.API.Core
{
    [Serializable]
    public class ReCraftedException : Exception
    {
        public ReCraftedException(string message) : base(message)
        {
        }
    }
}
