// ReCrafted (c) 2016-2018 Always Too Late

using System;
using System.Diagnostics;
using System.IO;

namespace ReCrafted.ProjectManager.Commands
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
                Console.WriteLine("Please specify the target toolchain with -t or --toolchain parameter! " +
                                  "Available options are: 'vs2015', 'vs2015' and 'vs2015'.");
                Environment.Exit(-1);
            }
            
            var process = new Process
            {
                StartInfo = new ProcessStartInfo(Path.Combine(Program.RootDirectory, $"tools\\{ProjectGenerator}.exe"))
                {
                    CreateNoWindow = true,
                    UseShellExecute = false,
                    RedirectStandardOutput = true,
                    RedirectStandardError = true
                }
            };

            var process1 = process;
            var startInfo = process1.StartInfo;
            startInfo.Arguments = string.Concat(startInfo.Arguments, " " + Options.Current.Toolchain);

            process1.Start();
            process1.WaitForExit();
            Console.WriteLine(process1.StandardError.ReadToEnd());
        }
    }
}
