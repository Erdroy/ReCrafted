// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

using System;
using System.Linq;

namespace ReCrafted.Tools.ProjectManager.Commands
{
    public class PostBuild : Command
    {
        public override void Execute()
        {
            // Compile shaders
            if (!Options.Current.SkipShaders)
                Program.RunCommand("CompileShaders");

            // Generate API
            Program.RunCommand("GenerateAPI");

            // Update build info
            Program.RunCommand("UpdateBuildInfo");

            var info = ProjectBuildInfo.FromFile(Program.BuildInfoFile);
            var buildNumber = info.Info.Sum(x => x.Value.BuildCount);

            // print some cool data
            Console.WriteLine($"==== Build done! Current build number ({info.BuildName}): {buildNumber}, " +
                              $"build time: {DateTime.Now.ToShortDateString()} {DateTime.Now.ToShortTimeString()} ====");
            Environment.Exit(0);
        }
    }
}
