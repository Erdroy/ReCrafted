// ReCrafted © 2016-2017 Always Too Late

using System;
using System.Collections.Generic;
using System.IO;
using System.Threading;

namespace ReCrafted.API.Core
{
    public delegate void MessageHandler(string message, LogLevel level);

    /// <summary>
    ///	Logger class. Writes debug messages to output file.
    /// </summary>
    public class Logger : IDisposable
    {
        private struct LogItem
        {
            public string Message { get; set; }
            public object Sender { get; set; }
            public LogLevel Level { get; set; }
            public DateTime Time { get; set; }
        }

        private static Logger _instance;

        private bool _disposed;
        private Thread _logThread;
        private FileStream _logStream;
        private StreamWriter _logWriter;
        private Queue<LogItem> _logQueue;

        internal void Init()
        {
            // create log queue
            _logQueue = new Queue<LogItem>();

            // create log thread
            _logThread = new Thread(ThreadRunner);
            _logThread.Start();

            if (File.Exists(LogFile))
            {
                // create backup directory if doesn't exist yet.
                if (!Directory.Exists("../logs"))
                    Directory.CreateDirectory("../logs");

                // TODO: compress file

                // move the file and change it's name
                File.Copy(LogFile, "../logs" + "/log_" + DateTime.Now.ToString("dd_MM_yyyy_HH_mm_ss") + ".txt");
            }

            // create log file stream
            _logStream = new FileStream(LogFile, FileMode.Create, FileAccess.ReadWrite, FileShare.ReadWrite);
            _logWriter = new StreamWriter(_logStream);
        }
        
        // private
        private void ThreadRunner()
        {
            while (!_disposed || (_disposed && _logQueue.Count > 0))
            {
                while (_logQueue.Count > 0) // process all logs
                {
                    // dequeue next log
                    var log = _logQueue.Dequeue();

                    // construct log message
                    var message = ConstructMessage(log);

                    // write log to the file
                    _logWriter.Write(message);
                    
                    // flush
                    _logStream.Flush();
                    _logWriter.Flush();
                }

                Thread.Sleep(LoggerThreadFrequency); // sleep some time to get some more new fresh logs to eat.
            }
        }

        private string ConstructMessage(LogItem log)
        {
            var level = LogLevelToString(log.Level);

            // construct log message
            var msg = log.Sender != null ? 
                $"{log.Time.ToString(TimeFormat)} [{level}] {log.Sender}: {log.Message}"
                : $"{log.Time.ToString(TimeFormat)} [{level}] {log.Message}";

            return msg.Replace("\n", "") + "\n";
        }
        
        /// <summary>
        /// Writes log.
        /// </summary>
        /// <param name="message">The message.</param>
        /// <param name="sender">The message sender(optional, use null when don't want to use this)</param>
        /// <param name="level">The log level.</param>
        public void Write(string message, object sender, LogLevel level)
        {
            if (!ProduceLogFile)
                return;

            // construct log
            var log = new LogItem
            {
                Level = level,
                Message = message,
                Sender = sender,
                Time = DateTime.Now
            };

            lock (_logQueue)
            {
                // enqueue the log message
                _logQueue.Enqueue(log);
            }

            if (UseLogCallback)
            {
                // try call OnMessage
                OnMessage?.Invoke(ConstructMessage(log), log.Level);
            }
        }


        /// <summary>
        /// Dispose logger.
        /// </summary>
        public void Dispose()
        {
            if (_disposed)
                return;

            _disposed = true;

            while (_logQueue.Count > 0)
                Thread.Sleep(5);

            _logStream.Dispose();
            _logWriter.Dispose();
            _logQueue.Clear();

            _instance = null;
        }
        
        private static void OnEngineLog(string message, LogLevel level)
        {
            Instance.Write(message, null, level);
        }

        private static void Shutdown()
        {
            Instance.Dispose();
        }
        
        public static void Log(string message, LogLevel level = LogLevel.Info)
        {
            Instance.Write(message, null, level);
        }
        
        public static void LogWarning(string message)
        {
            Instance.Write(message, null, LogLevel.Warning);
        }

        public static void LogError(string message)
        {
            Instance.Write(message, null, LogLevel.Error);
        }

        public static void LogFatal(string message)
        {
            Instance.Write(message, null, LogLevel.Fatal);
        }

        internal static string LogLevelToString(LogLevel level)
        {
            switch (level)
            {
                case LogLevel.Info:
                    return "INFO";
                case LogLevel.Warning:
                    return "WARNING";
                case LogLevel.Error:
                    return "ERROR";
                case LogLevel.Fatal:
                    return "FATAL";
                default:
                    throw new ArgumentOutOfRangeException(nameof(level), level, null);
            }
        }

        /// <summary>
        /// The logger instance.
        /// </summary>
        public static Logger Instance
        {
            get
            {
                if (_instance != null)
                    return _instance;

                _instance = new Logger();
                _instance.Init();
                return _instance;
            }
        }

        /// <summary>
        /// Should MyLogger call OnMessage when new log is being ::Write?
        /// </summary>
        public static bool UseLogCallback { get; set; } = true;

        /// <summary>
        /// Can MyLogger produce log file?
        /// </summary>
        public static bool ProduceLogFile { get; set; } = true;

        /// <summary>
        /// How frequently(in miliseconds) the logger thread(if enabled) should try to flush logs.
        /// </summary>
        public static int LoggerThreadFrequency { get; set; } = 10;

        /// <summary>
        /// Current time format.
        /// </summary>
        public static string TimeFormat { get; set; } = "dd.MM.yyyy HH:mm:ss";

        /// <summary>
        /// The current log file name.
        /// </summary>
        public static string LogFile { get; } = "../logs/log.txt";

        /// <summary>
        /// OnMessage event - called when new log is queued.
        /// MySettings.UseLogCallback = true; is required!
        /// </summary>
        public static event MessageHandler OnMessage;
    }
}
