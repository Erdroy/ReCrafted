// ReCrafted (c) 2016-2019 Always Too Late. All rights reserved.

using System;
using System.IO;
using Newtonsoft.Json;

namespace ReCrafted.ProjectManager.Commands
{
    public class SetupProject : Command
    {
        public override void Execute()
        {
            if (string.IsNullOrEmpty(Options.Current.Username))
            {
                Console.WriteLine("Please specify username with -u or --username parameter!");
                Environment.Exit(-1);
                return;
            }

            var developerInfo = new DeveloperInfo
            {
                Username = Options.Current.Username
            };

            File.WriteAllText(Program.DeveloperInfoFile, developerInfo.ToJson(Formatting.Indented));
        }
    }
}
