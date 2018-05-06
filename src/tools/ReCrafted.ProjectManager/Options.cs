// ReCrafted (c) 2016-2018 Always Too Late

using CommandLine;

namespace ReCrafted.ProjectManager
{
    public class Options
    {
        [Option('c', "command", Required = true, HelpText = "The base command.")]
        public string Command { get; set; }
        
        [Option('u', "username", Required = false, HelpText = "Used with SetupProject command.")]
        public string Username { get; set; }

        [Option('t', "toolchain", Required = false, HelpText = "The target tool chain to generate project for. Used with CreateProject command.")]
        public string Toolchain { get; set; }

        [Option("skip-shaders", Required = false, HelpText = "When added, the shader compilation will be skipped while PostBuild command is executed.")]
        public bool SkipShaders { get; set; }

        public static Options Current { get; set; }
    }
}
