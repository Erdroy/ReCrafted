// ReCrafted (c) 2016-2019 Always Too Late

using System;

namespace ReCrafted.API.Core
{
    internal static class Exceptions
    {
        /// <summary>
        /// Registers Unhandled Exception handler.
        /// </summary>
        public static void RegisterUEHandler()
        {
            Logger.Log("Registered UnhandledException handler");

            AppDomain.CurrentDomain.UnhandledException += (sender, args) =>
            {
                var exception = args.ExceptionObject as Exception;

                if (exception != null)
                    WriteException(exception);
            };
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
            Logger.Log($"Unhandled Exception: {GetExceptionString(exception)}", LogLevel.Error);

            // check for inner exception
            if (exception.InnerException == null)
                return;

            exception = exception.InnerException;

            // handle all inner exceptions
            while (exception != null)
            {
                Logger.Log($"Inner Exception: {GetExceptionString(exception)}", LogLevel.Error);

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
