using System;
using System.Diagnostics;

namespace ReCrafted.ShaderCompiler
{
    public static class ShaderCompiler
    {
        public static bool CompileFile(string input, string output)
        {
            var perfCounter = new Stopwatch();
            perfCounter.Start();
            var compiler = new Compiler.ShaderCompiler(input, output);

            try
            {
                var perfCounterParse = new Stopwatch();
                perfCounterParse.Start();
                compiler.Parse();
                perfCounterParse.Stop();
                Console.WriteLine("Parsing done in " + perfCounterParse.ElapsedMilliseconds + " ms.");

                perfCounterParse.Reset();
                perfCounterParse.Start();
                compiler.Compile();
                compiler.Save();
                perfCounterParse.Stop();
                Console.WriteLine("Compilation done in " + perfCounterParse.ElapsedMilliseconds + " ms.");
            }
            catch (Exception ex)
            {
                var errorMessage = Options.Current.Verbose ? ex.ToString() : ex.Message;
                Console.WriteLine("Failed to compile shader.\nError: \n" + errorMessage);
                return false;
            }

            // say bye and give the time that we spent here
            perfCounter.Stop();
            Console.WriteLine("Done in " + perfCounter.ElapsedMilliseconds + " ms.");
            Console.WriteLine("Successfully compiled shader. Output: " + output);
            return true;
        }
    }
}
