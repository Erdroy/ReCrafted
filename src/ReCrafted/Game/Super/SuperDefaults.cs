// ReCrafted © 2016-2017 Always Too Late

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

            SuperCommands.Register("", new[] {"exit", "quit", "close"}, API.Core.Game.Quit, "Close ReCrafted.");
        }
    }
}
