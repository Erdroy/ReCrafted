// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

using System;
using System.Diagnostics;
using System.IO;

namespace ReCrafted.Tools.ProjectManager.Commands
{
    public class CreateProjects : Command
    {
        public readonly string ProjectGenerator = "premake5";

        public override void Execute()
        {
            if (!File.Exists(Program.DeveloperInfoFile))
            {
                Console.WriteLine("Please run SetupProject command first!");
                Environment.Exit(-1);
            }

            if (string.IsNullOrEmpty(Options.Current.Toolchain))
            {
                Console.WriteLine("Next time, please specify the target toolchain with -t or --toolchain parameter! " +
                                  "Available options are: 'vs2015', 'vs2017' and 'vs2019'.");

                Options.Current.Toolchain = "vs2019";
            }

            Console.WriteLine("Creating projects for " + Options.Current.Toolchain);
            
            var process = new Process
            {
                StartInfo = new ProcessStartInfo(Path.Combine(Program.RootDirectory, $"Tools\\{ProjectGenerator}.exe"))
                {
                    CreateNoWindow = true,
                    UseShellExecute = false,
                    RedirectStandardOutput = true,
                    RedirectStandardError = true
                }
            };

            var startInfo = process.StartInfo;
            startInfo.Arguments = string.Concat(startInfo.Arguments, " " + Options.Current.Toolchain);

            process.Start();
            process.WaitForExit();
            Console.WriteLine(process.StandardOutput.ReadToEnd());
            Console.WriteLine(process.StandardError.ReadToEnd());
        }
    }
}
