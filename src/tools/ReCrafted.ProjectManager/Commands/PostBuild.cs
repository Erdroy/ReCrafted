// ReCrafted (c) 2016-2018 Always Too Late

using System;
using System.Linq;

namespace ReCrafted.ProjectManager.Commands
{
    public class PostBuild : Command
    {
        public override void Execute()
        {
            // compile shaders
            if (!Options.Current.SkipShaders)
                Program.RunCommand("CompileShaders");

            // generate api
            if (!Options.Current.SkipAPI)
                Program.RunCommand("GenerateAPI");

            // update build info
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
