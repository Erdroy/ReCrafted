// ReCrafted (c) 2016-2019 Always Too Late. All rights reserved.

using System;
using System.Windows.Forms;

namespace ReCrafted.AtlasPacker
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new MainForm());
        }
    }
}
