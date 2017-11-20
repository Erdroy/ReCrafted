// ReCrafted © 2016-2017 Always Too Late

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
        public static void Write(string message, LogLevel level = LogLevel.Info)
        {
            Internal_Write(message, level);
        }
    }
}
