// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

namespace ReCrafted.API.Core
{
    /// <summary>
    ///	Logger class. Writes debug messages to output file.
    /// </summary>
    public static partial class Logger
    {
        /// <summary>
        ///	Writes message to the output file.
        /// </summary>
        public static void Write(string message, params object[] parameters)
        {
            Write(string.Format(message, parameters));
        }
    }
}
