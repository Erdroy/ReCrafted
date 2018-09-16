// ReCrafted Editor (c) 2016-2018 Always Too Late

using System;

namespace ReCrafted.Editor.Common
{
    public class Time
    {
        private static DateTime _lastUpdate;
        private static DateTime _initTime;

        public static void Init()
        {
            _initTime = DateTime.Now;
        }

        public static void Update()
        {
            var current = DateTime.Now;
            var diff = current - _lastUpdate;
            DeltaTime = (float)diff.TotalSeconds;
            CurrentTime = (float)(current - _initTime).TotalSeconds;
            _lastUpdate = current;
        }

        public static float CurrentTime { get; private set; }
        public static float DeltaTime { get; private set; }
    }
}
