// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

namespace ReCrafted.API.Common
{
    public static partial class Logger
    {
        public static void Log(string str, LogLevel level)
        {
            InternalWriteLog(level, str);
        }

        public static void Log(string str)
        {
            InternalWriteLog(LogLevel.Message, str);
        }

        public static void LogWarning(string str)
        {
            InternalWriteLog(LogLevel.Warning, str);
        }

        public static void LogError(string str)
        {
            InternalWriteLog(LogLevel.Error, str);
        }

        public static void LogFatal(string str)
        {
            InternalWriteLog(LogLevel.Fatal, str);
        }

        public static void LogAssert(string str)
        {
            InternalWriteLog(LogLevel.Assert, str);
        }
    }
}
