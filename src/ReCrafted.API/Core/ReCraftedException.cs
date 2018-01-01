// ReCrafted (c) 2016-2018 Always Too Late

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
