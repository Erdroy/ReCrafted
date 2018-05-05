// ReCrafted (c) 2016-2018 Always Too Late

using System;
using CommandLine;

namespace ReCrafted.ProjectManager
{
    internal class Program
    {
        public static string RootDirectory { get; private set; }

        private static void Main(string[] args)
        {
            Parser.Default.ParseArguments<Options>(args)
                .WithParsed(opts =>
                {
                    Options.Current = opts;
                });

            if (Options.Current == null)
                return;

            RootDirectory = Environment.CurrentDirectory;
            RunCommand(Options.Current.Command);
            Console.WriteLine("Done");
        }

        /// <summary>
        /// Runs command of given name.
        /// </summary>
        /// <param name="commandName"></param>
        public static void RunCommand(string commandName)
        {
            foreach (var command in ReflectiveEnumerator.GetEnumerableOfType<Command>())
            {
                if (command.Name != commandName)
                    continue;

                // execute command
                command.Execute();
                return;
            }

            Console.WriteLine($"Command '{commandName}' not found!");
            Environment.Exit(-1);
        }
    }
}
