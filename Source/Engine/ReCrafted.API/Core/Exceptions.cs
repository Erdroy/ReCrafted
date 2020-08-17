// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

using System;
using ReCrafted.API.Common;

namespace ReCrafted.API.Core
{
    public static class Exceptions
    {
        /// <summary>
        /// Registers Unhandled Exception handler.
        /// </summary>
        public static void RegisterUEHandler()
        {
            AppDomain.CurrentDomain.UnhandledException += (sender, args) =>
            {
                if (args.ExceptionObject is Exception exception)
                    WriteException(exception);
            };

            Logger.Log("Registered UnhandledException handler");
        }

        /// <summary>
        /// Throws exception, used by engine to throw unhandled exceptions.
        /// </summary>
        /// <param name="ex">The exception which will be thrown.</param>
        public static void ThrowException(Exception ex)
        {
            throw ex;
        }

        // private
        internal static void WriteException(Exception exception)
        {
            Logger.Log($"{GetExceptionString(exception)}", LogLevel.Exception);

            // check for inner exception
            if (exception.InnerException == null)
                return;

            exception = exception.InnerException;

            // handle all inner exceptions
            while (exception != null)
            {
                Logger.Log($"Inner Exception: {GetExceptionString(exception)}", LogLevel.Exception);

                if (exception.InnerException == null)
                    return;

                exception = exception.InnerException;
            }
        }

        // private
        private static string GetExceptionString(Exception ex)
        {
            var str = ex.ToString();
            str = str.Replace("\r", "");
            return str;
        }
    }
}
