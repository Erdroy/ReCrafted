// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using CommandLine;

namespace ReCrafted.Tools.ShaderCompiler
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

            // compile shader
            ShaderCompiler.CompileFile(Options.Current.InputFile, Options.Current.OutputFile);
        }
    }
}
