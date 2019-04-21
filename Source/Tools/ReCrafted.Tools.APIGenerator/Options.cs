// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using CommandLine;

namespace ReCrafted.Tools.APIGenerator
{
    public class Options
    {
        [Option('i', "input", Required = true, HelpText = "Input file name.")]
        public string InputFile { get; set; }

        [Option('o', "csout", Required = true, HelpText = "Output C# file name.")]
        public string CSharpOutputFile { get; set; }

        [Option('c', "cppout", Required = true, HelpText = "Output C# file name.")]
        public string CPlusPlusOutputFile { get; set; }

        [Option('v', "verbose", Default = false, Required = false)]
        public bool Verbose { get; set; }

        public static Options Current { get; set; }
    }
}