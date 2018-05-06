// ReCrafted (c) 2016-2018 Always Too Late

using CommandLine;

namespace ReCrafted.ShaderCompiler
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
