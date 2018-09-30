// ReCrafted Editor (c) 2016-2018 Always Too Late

using System;
using System.IO;

namespace ReCrafted.Editor.Common
{
    public class Logger
    {
        public enum LogType
        {
            Info,
            Warning,
            Error,
            Exception
        }

        private static Logger _instance;
        private readonly FileStream _fileStream;
        private readonly StreamWriter _stream;

        public Logger()
        {
            _fileStream = new FileStream("log.txt", FileMode.Create, FileAccess.Write, FileShare.ReadWrite);
            _stream = new StreamWriter(_fileStream);
        }

        ~Logger()
        {
            _fileStream.Dispose();
        }

        protected virtual void OnLogWritten(string obj, LogType logType)
        {
            LogWritten?.Invoke(obj, logType);
        }

        public static void Log(string message, LogType logType)
        {
            var logMessage = $"{DateTime.Now.ToString("dd.MM.yyyy HH:mm:ss")} [{logType.ToString()}]: {message}\n";
            Instance._stream.Write(logMessage);
            Instance.OnLogWritten(logMessage, logType);
        }

        public static void Log(string message)
        {
            Log(message, LogType.Info);
        }

        public static void LogWarning(string message)
        {
            Log(message, LogType.Warning);
        }

        public static void LogError(string message)
        {
            Log(message, LogType.Error);
        }

        public static void LogException(Exception exception)
        {
            // TODO: Format exception
            Log(exception.Message, LogType.Exception);
        }

        public event Action<string, LogType> LogWritten;

        public static Logger Instance => _instance ?? (_instance = new Logger());
    }
}
