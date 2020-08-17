// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

using System;

namespace ReCrafted.API.Core
{
    [Serializable]
    public class MissingReferenceException : Exception
    {
        public MissingReferenceException() { }

        public MissingReferenceException(string message) : base(message)
        {
        }
    }
}