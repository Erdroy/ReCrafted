// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

using System;
using ReCrafted.API.Core;

namespace ReCrafted.Game.Core
{
    internal static class Exceptions
    {
        /// <summary>
        /// Registers Unhandled Exception handler.
        /// </summary>
        public static void RegisterUEHandler()
        {
            Logger.Write("Registered UnhandledException handler");

            AppDomain.CurrentDomain.UnhandledException += (sender, args) =>
            {
                var exception = args.ExceptionObject as Exception;

                if (exception != null)
                    WriteException(exception);
            };
        }

        // private
        private static void WriteException(Exception exception)
        {
            Logger.Write($"Unhandled Exception: {exception}", LogLevel.Error);

            // check for inner exception
            if (exception.InnerException == null)
                return;

            exception = exception.InnerException;

            // handle all inner exceptions
            while (exception != null)
            {
                Logger.Write($"Inner Exception: {exception}", LogLevel.Error);

                if (exception.InnerException == null)
                    return;

                exception = exception.InnerException;
            }
        }
    }
}
