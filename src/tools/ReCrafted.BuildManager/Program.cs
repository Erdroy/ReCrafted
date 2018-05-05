// ReCrafted (c) 2016-2018 Always Too Late

using System;
using CommandLine;

namespace ReCrafted.BuildManager
{
    internal class Program
    {
        //private static Type[] BuildCommands = new[] { typeof(BuildShaders)};

        private static void Main(string[] args)
        {
            Parser.Default.ParseArguments<Options>(args)
                .WithParsed(opts =>
                {
                    Options.Current = opts;
                });

            if (Options.Current == null)
                return;

            //RunCommand(Options.Current.Command);
        }
    }
}
