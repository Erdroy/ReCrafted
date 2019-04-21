// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using CommandLine;

namespace ReCrafted.Tools.ShaderCompiler
{
    public class Options
    {
        [Option('i', "input", Required = true, HelpText = "Input file name.")]
        public string InputFile { get; set; }

        [Option('o', "output", Required = true, HelpText = "Output file name.")]
        public string OutputFile { get; set; }
        
        [Option('d', "disassemble", Default = false, Required = false)]
        public bool DisassembleOutput { get; set; }
        
        [Option('v', "verbose", Default = false, Required = false)]
        public bool Verbose { get; set; }

        [Option("cute", Default = false, Required = false, HelpText = "Outputs JSON header as cute as possible. owo")]
        public bool CuteJson { get; set; }

        public static Options Current { get; set; }
    }
}
