// ReCrafted (c) 2016-2018 Always Too Late

using System;
using System.Diagnostics;
using CommandLine;

namespace ReCrafted.ShaderCompiler
{
    internal class Program
    {
        private static void Main(string[] args)
        {
            var perfCounter = new Stopwatch();
            perfCounter.Start();

            Parser.Default.ParseArguments<Options>(args)
                .WithParsed(opts =>
                {
                    Options.Current = opts;
                });

            if (Options.Current == null)
                return;

            var compiler = new Compiler.ShaderCompiler(Options.Current.InputFile, Options.Current.OutputFile);

            try
            {
                var perfCounterParse = new Stopwatch();
                perfCounterParse.Start();
                compiler.Parse();
                perfCounterParse.Stop();
                Console.WriteLine("Parsing done in " + perfCounterParse.ElapsedMilliseconds + " ms.");

                compiler.Compile();
                compiler.Save();
            }
            catch (Exception ex)
            {
                var errorMessage = Options.Current.Verbose ? ex.ToString() : ex.Message;
                Console.WriteLine("Failed to parse shader.\nError: " + errorMessage);
                Console.ReadLine();
                return;
            }
            
            // say bye and give the time that we spent here
            perfCounter.Stop();
            Console.WriteLine("Done in " + perfCounter.ElapsedMilliseconds + " ms.");
            Console.WriteLine("Successfully compiled shader. Output: " + Options.Current.OutputFile);
        }
    }
}
