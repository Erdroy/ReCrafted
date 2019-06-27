// ReCrafted Editor (c) 2016-2018 Always Too Late

using System;
using ReCrafted.Editor.Core;

namespace ReCrafted.Editor
{
    internal static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        private static void Main()
        {
            using (var app = new EditorApplication())
            {
                app.Run();
            }
        }
    }
}
