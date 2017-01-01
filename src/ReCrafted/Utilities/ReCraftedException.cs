// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

using System;

namespace ReCrafted.Utilities
{
    /// <summary>
    /// ReCraftedException class.
    /// </summary>
    public sealed class ReCraftedException : Exception
    {
        /// <summary>
        /// ReCraftedException constructor.
        /// </summary>
        /// <param name="message">The message</param>
        public ReCraftedException(string message) : base(message) { }
    }
}