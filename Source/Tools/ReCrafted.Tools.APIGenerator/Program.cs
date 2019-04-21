// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using CommandLine;

namespace ReCrafted.Tools.APIGenerator
{
    internal class Program
    {
        private static void Main(string[] args)
        {
            Parser.Default.ParseArguments<Options>(args)
                .WithParsed(opts =>
                {
                    Options.Current = opts;
                });

            if (Options.Current == null)
                return;

            // Generate C# and C++ class from given header file
            Generator.GenerateClass(Options.Current.InputFile, Options.Current.CSharpOutputFile, Options.Current.CPlusPlusOutputFile);
        }
    }
}