// ReCrafted (c) 2016-2018 Always Too Late

using ReCrafted.API.Core;
using ReCrafted.API.Mathematics;

namespace ReCrafted.Game.Super
{
    /// <summary>
    /// Default commands of the game.
    /// </summary>
    public sealed class SuperDefaults
    {
        /// <summary>
        /// Register default commands.
        /// </summary>
        public static void RegisterSuperDefaults()
        {
            // help
            SuperCommands.Register("", "help", () =>
            {
                var commands = SuperCommands.Instance.GetAllCommands();
                var text = SuperConsole.Write($"List of {commands.Length} commands.");
                text.TextColor = Color.DarkOrange;
                foreach (var command in commands)
                {
                    text = SuperConsole.Write($"{string.Join("|", command.Names)} - {command.Description}");
                    text.TextColor = Color.DarkOrange;
                }
            }, "Prints all commands.");

            // exit
            SuperCommands.Register("", new[] {"exit", "quit", "close"}, API.Core.Game.Quit, "Close ReCrafted.");

            // debug
            SuperCommands.Register("", "debug", () =>
            {
                SuperConsole.Write("Hello Info", LogLevel.Info);
                SuperConsole.Write("Hello Warning", LogLevel.Warning);
                SuperConsole.Write("Hello Error", LogLevel.Error);
                SuperConsole.Write("Hello Fatal", LogLevel.Fatal);
            }, "Prints all types of log in console.");

            // clear
            SuperCommands.Register("", "clear", SuperConsole.Clear, "Clears console.");
        }
    }
}
