// ReCrafted (c) 2016-2018 Always Too Late

using System;
using System.IO;
using System.Linq;
using Newtonsoft.Json;
using ReCrafted.Common;

namespace ReCrafted.ProjectManager.Commands
{
    public class UpdateBuildInfo : Command
    {
        public override void Execute()
        {
            if (!File.Exists(Program.DeveloperInfoFile))
            {
                Console.WriteLine("Please run SetupProject command first!");
                Environment.Exit(-1);
            }

            if (!File.Exists(Program.BuildInfoFile))
            {
                Console.WriteLine("buildinfo.json file is missing! " +
                                  "Make sure, that working directory is the root directory of the project.");
                Environment.Exit(-1);
            }

            var developerInfo = DeveloperInfo.FromFile(Program.DeveloperInfoFile);
            var buildInfo = ProjectBuildInfo.FromFile(Program.BuildInfoFile);

            // increment build count for current developer
            buildInfo.BuildCounts[developerInfo.Username] = buildInfo.BuildCounts[developerInfo.Username] + 1;

            // set latest build info
            buildInfo.LastBuild = DateTime.Now;

            // ... and now save the modified file
            buildInfo.ToFile(Program.BuildInfoFile, Formatting.Indented);

            // sum all build counts and export it to .\build\game\assets\gameinfo.json
            var gameInfo = new GameInfo
            {
                BuildAuthor = developerInfo.Username,
                BuildName = buildInfo.BuildName,
                BuildNumber = buildInfo.BuildCounts.Sum(x => x.Value),
                BuildTime = buildInfo.LastBuild
            };

            // save now
            gameInfo.ToFile(Program.GameInfoFile, Formatting.Indented);
        }
    }
}
