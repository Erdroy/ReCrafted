// ReCrafted (c) 2016-2018 Always Too Late

using CommandLine;

namespace ReCrafted.BuildManager
{
    public class Options
    {
        [Option(Required = true, HelpText = "Input command")]
        public string Command { get; set; }
        
        public static Options Current { get; set; }
    }
}
