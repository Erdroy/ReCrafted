// ReCrafted (c) 2016-2018 Always Too Late

using CommandLine;

namespace ReCrafted.ProjectManager
{
    public class Options
    {
        [Option('c', "command", Required = true, HelpText = "Input command")]
        public string Command { get; set; }
        
        [Option('u', "username", Required = false, HelpText = "Used with SetupProject command")]
        public string Username { get; set; }

        public static Options Current { get; set; }
    }
}
