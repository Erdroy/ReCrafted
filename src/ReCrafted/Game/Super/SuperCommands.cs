// ReCrafted (c) 2016-2018 Always Too Late

using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using ReCrafted.API.Core;

namespace ReCrafted.Game.Super
{
    /// <summary>
    /// Allows to register commands.
    /// </summary>
    public sealed class SuperCommands
    {
        /// <summary>
        /// Structure for command data.
        /// </summary>
        public struct Command
        {
            public string[] Names { get; set; }
            public string Description { get; set; }
            public string Group { get; set; }
            public ParameterInfo[] Parameters { get; set; }
            public object MethodTarget { get; set; }
            public MethodInfo Method { get; set; }
        }

        // private
        private readonly List<Command> _commands = new List<Command>();

        /// <summary>
        /// Registers command with specified name and execution method in given command group.
        /// </summary>
        /// <param name="commandGroup">The command group, easily unregister batch of commands by one call.
        /// Use this of eg.: level dependent commands, only 'when playing' command etc. </param>
        /// <param name="commandName">The command name.</param>
        /// <param name="onExecute">Called when command is being executed.</param>
        /// <param name="description">(optional)The command description.</param>
        public static void Register(string commandGroup, string commandName, Action onExecute,
            string description = "No description.")
        {
            Instance.RegisterCommandInternal(commandGroup, new[] {commandName}, onExecute.Target, onExecute.Method,
                description);
        }

        /// <summary>
        /// Registers command with specified name and execution method in given command group.
        /// </summary>
        /// <param name="commandGroup">The command group, easily unregister batch of commands by one call.
        /// Use this of eg.: level dependent commands, only 'when playing' command etc. </param>
        /// <param name="commandName">The command name.</param>
        /// <param name="onExecute">Called when command is being executed.</param>
        /// <param name="description">(optional)The command description.</param>
        public static void Register<T1>(string commandGroup, string commandName, Action<T1> onExecute,
            string description = "No description.")
        {
            Instance.RegisterCommandInternal(commandGroup, new[] {commandName}, onExecute.Target, onExecute.Method,
                description);
        }

        /// <summary>
        /// Registers command with specified name and execution method in given command group.
        /// </summary>
        /// <param name="commandGroup">The command group, easily unregister batch of commands by one call.
        /// Use this of eg.: level dependent commands, only 'when playing' command etc. </param>
        /// <param name="commandName">The command name.</param>
        /// <param name="onExecute">Called when command is being executed.</param>
        /// <param name="description">(optional)The command description.</param>
        public static void Register<T1, T2>(string commandGroup, string commandName, Action<T1, T2> onExecute,
            string description = "No description.")
        {
            Instance.RegisterCommandInternal(commandGroup, new[] {commandName}, onExecute.Target, onExecute.Method,
                description);
        }

        /// <summary>
        /// Registers command with specified name and execution method in given command group.
        /// </summary>
        /// <param name="commandGroup">The command group, easily unregister batch of commands by one call.
        /// Use this of eg.: level dependent commands, only 'when playing' command etc. </param>
        /// <param name="commandName">The command name.</param>
        /// <param name="onExecute">Called when command is being executed.</param>
        /// <param name="description">(optional)The command description.</param>
        public static void Register<T1, T2, T3>(string commandGroup, string commandName, Action<T1, T2, T3> onExecute,
            string description = "No description.")
        {
            Instance.RegisterCommandInternal(commandGroup, new[] {commandName}, onExecute.Target, onExecute.Method,
                description);
        }

        /// <summary>
        /// Registers command with specified name and execution method in given command group.
        /// </summary>
        /// <param name="commandGroup">The command group, easily unregister batch of commands by one call.
        /// Use this of eg.: level dependent commands, only 'when playing' command etc. </param>
        /// <param name="commandName">The command name.</param>
        /// <param name="onExecute">Called when command is being executed.</param>
        /// <param name="description">(optional)The command description.</param>
        public static void Register<T1, T2, T3, T4>(string commandGroup, string commandName,
            Action<T1, T2, T3, T4> onExecute, string description = "No description.")
        {
            Instance.RegisterCommandInternal(commandGroup, new[] {commandName}, onExecute.Target, onExecute.Method,
                description);
        }

        /// <summary>
        /// Registers command with specified name and execution method in given command group.
        /// </summary>
        /// <param name="commandGroup">The command group, easily unregister batch of commands by one call.
        /// Use this of eg.: level dependent commands, only 'when playing' command etc. </param>
        /// <param name="commandNames">The command names.</param>
        /// <param name="onExecute">Called when command is being executed.</param>
        /// <param name="description">(optional)The command description.</param>
        public static void Register(string commandGroup, string[] commandNames, Action onExecute,
            string description = "No description.")
        {
            Instance.RegisterCommandInternal(commandGroup, commandNames, onExecute.Target, onExecute.Method,
                description);
        }

        /// <summary>
        /// Registers command with specified name and execution method in given command group.
        /// </summary>
        /// <param name="commandGroup">The command group, easily unregister batch of commands by one call.
        /// Use this of eg.: level dependent commands, only 'when playing' command etc. </param>
        /// <param name="commandNames">The command names.</param>
        /// <param name="onExecute">Called when command is being executed.</param>
        /// <param name="description">(optional)The command description.</param>
        public static void Register<T1>(string commandGroup, string[] commandNames, Action<T1> onExecute,
            string description = "No description.")
        {
            Instance.RegisterCommandInternal(commandGroup, commandNames, onExecute.Target, onExecute.Method,
                description);
        }

        /// <summary>
        /// Registers command with specified name and execution method in given command group.
        /// </summary>
        /// <param name="commandGroup">The command group, easily unregister batch of commands by one call.
        /// Use this of eg.: level dependent commands, only 'when playing' command etc. </param>
        /// <param name="commandNames">The command names.</param>
        /// <param name="onExecute">Called when command is being executed.</param>
        /// <param name="description">(optional)The command description.</param>
        public static void Register<T1, T2>(string commandGroup, string[] commandNames, Action<T1, T2> onExecute,
            string description = "No description.")
        {
            Instance.RegisterCommandInternal(commandGroup, commandNames, onExecute.Target, onExecute.Method,
                description);
        }

        /// <summary>
        /// Registers command with specified name and execution method in given command group.
        /// </summary>
        /// <param name="commandGroup">The command group, easily unregister batch of commands by one call.
        /// Use this of eg.: level dependent commands, only 'when playing' command etc. </param>
        /// <param name="commandNames">The command names.</param>
        /// <param name="onExecute">Called when command is being executed.</param>
        /// <param name="description">(optional)The command description.</param>
        public static void Register<T1, T2, T3>(string commandGroup, string[] commandNames,
            Action<T1, T2, T3> onExecute,
            string description = "No description.")
        {
            Instance.RegisterCommandInternal(commandGroup, commandNames, onExecute.Target, onExecute.Method,
                description);
        }

        /// <summary>
        /// Registers command with specified name and execution method in given command group.
        /// </summary>
        /// <param name="commandGroup">The command group, easily unregister batch of commands by one call.
        /// Use this of eg.: level dependent commands, only 'when playing' command etc. </param>
        /// <param name="commandNames">The command names.</param>
        /// <param name="onExecute">Called when command is being executed.</param>
        /// <param name="description">(optional)The command description.</param>
        public static void Register<T1, T2, T3, T4>(string commandGroup, string[] commandNames,
            Action<T1, T2, T3, T4> onExecute, string description = "No description.")
        {
            Instance.RegisterCommandInternal(commandGroup, commandNames, onExecute.Target, onExecute.Method,
                description);
        }

        // internal
        internal void RegisterCommandInternal(string commandGroup, string[] commandNames, object instance,
            MethodInfo method, string description)
        {
            var parameters = method.GetParameters();

            // check commands, do not allow duplicates!
            var commands = _commands.Where(x =>
                    x.Names.Intersect(commandNames).Any() && x.Parameters.Length == parameters.Length)
                .ToArray();
            if (commands.Length != 0)
            {
                SuperConsole.Write(
                    "Command with one of this names (" + string.Join(" ", commandNames) +
                    ") and the same parameters count already exists.", LogLevel.Error);
                return;
            }

            // register command
            _commands.Add(new Command
            {
                Names = commandNames,
                Description = description,
                Group = commandGroup,
                Parameters = parameters,
                MethodTarget = instance,
                Method = method
            });
        }

        /// <summary>
        /// Unregisters batch of commands by group name.
        /// </summary>
        /// <param name="commandGroup">The group name.</param>
        public void UnregisterGroup(string commandGroup)
        {
            // select commands with given group name which will be deleted
            var toRemove = _commands.Where(x => x.Group == commandGroup);

            // delete all selected commands
            foreach (var remove in toRemove)
                _commands.Remove(remove);
        }

        /// <summary>
        /// Executes command with given parameters.
        /// </summary>
        /// <param name="commandName">The command name.</param>
        /// <param name="parameters">The command parameters, typeless, checking and parsing will be done.</param>
        public void Execute(string commandName, params string[] parameters)
        {
            // check
            var commands = _commands.Where(x => x.Names.Contains(commandName)).ToArray();

            if (commands.Length == 0)
            {
                Logger.LogError("'" + commandName + "' command not found.");
                return;
            }

            var found = false;
            var command = new Command();
            foreach (var cmd in commands)
            {
                if (cmd.Parameters.Length != parameters.Length)
                    continue;

                found = true;
                command = cmd;
                break;
            }

            if (!found)
            {
                Logger.LogError("'" + commandName + "' command exists, but invalid parameters were given." + parameters.Length);
                return;
            }

            // parse
            var cmdParams = command.Parameters;
            var paramIndex = 0;
            var parseParams = new object[parameters.Length];

            foreach (var parameter in parameters)
            {
                var cmdParameter = cmdParams[paramIndex];

                switch (cmdParameter.ParameterType.Name.ToLower())
                {
                    case "string":
                        // string does not need any type check
                        parseParams[paramIndex] = parameter;
                        break;
                    case "int32":
                        int resultInt;
                        if (int.TryParse(parameter, out resultInt))
                        {
                            parseParams[paramIndex] = resultInt;
                        }
                        else
                        {
                            Logger.LogError("invalid parameter type were given for '" + parameter + "'");
                            return;
                        }
                        break;
                    case "single":
                        float resultSingle;
                        if (float.TryParse(parameter, out resultSingle))
                        {
                            parseParams[paramIndex] = resultSingle;
                        }
                        else
                        {
                            Logger.LogError("invalid parameter type were given for '" + parameter + "'");

                            return;
                        }
                        break;
                    case "double":
                        double resultDouble;
                        if (double.TryParse(parameter, out resultDouble))
                        {
                            parseParams[paramIndex] = resultDouble;
                        }
                        else
                        {
                            Logger.LogError("invalid parameter type were given for '" + parameter + "'");
                            return;
                        }
                        break;
                    case "boolean":
                        bool resultBoolean;
                        if (bool.TryParse(parameter, out resultBoolean))
                        {
                            parseParams[paramIndex] = resultBoolean;
                        }
                        else
                        {
                            Logger.LogError("invalid parameter type were given for '" + parameter + "'");
                            return;
                        }
                        break;
                    default:
                        Logger.LogError("command target method has invalid type in parameters!");
                        return;
                }

                paramIndex++;
            }

            // execute!
            command.Method.Invoke(command.MethodTarget, parseParams);
        }

        /// <summary>
        /// Executes raw command, parse arguments etc.
        /// </summary>
        /// <param name="command">The command string, eg.: 'volume master 0.2' or 'volume "master" 0.2'</param>
        public void ExecuteRaw(string command)
        {
            // parse
            string name;
            var parameters = ParseCommand(command, out name);

            // execute
            Execute(name, parameters.ToArray());
        }

        /// <summary>
        /// Lists all available commands.
        /// </summary>
        /// <returns>The commands array containing all commands.</returns>
        public Command[] GetAllCommands()
        {
            return _commands.ToArray();
        }

        /// <summary>
        /// Initializes the MyCommand instance and internal commands.
        /// </summary>
        public static void Init()
        {
            if (Instance != null)
            {
                Logger.LogError("SuperCommands can be initialized only once!");
                return;
            }

            // create instance
            Instance = new SuperCommands();
        }

        /// <summary>
        /// Parse command to get name and parameters.
        /// Strings are supported! eg.: 'test "Hello, World!"'
        /// </summary>
        /// <param name="command"></param>
        /// <param name="name"></param>
        /// <returns></returns>
        public static List<string> ParseCommand(string command, out string name)
        {
            // trim
            command = command.Trim();

            name = "";

            var parameters = new List<string>();
            var parameter = "";
            var stringRead = false;

            foreach (var ch in command)
            {
                if (ch == ' ' && !stringRead)
                {
                    // next param

                    if (!string.IsNullOrEmpty(parameter))
                        parameters.Add(parameter);

                    parameter = string.Empty;
                }
                else if (ch == '"')
                {
                    // start or stop string param   
                    if (stringRead)
                    {
                        stringRead = false;
                        continue;
                    }

                    stringRead = true;
                }
                else
                {
                    // add to current.
                    parameter += ch;
                }
            }

            // add last parameter
            if (!string.IsNullOrEmpty(parameter))
            {
                parameters.Add(parameter);
            }

            // set name
            if (string.IsNullOrEmpty(name) && parameters.Count > 0)
            {
                name = parameters[0];
                parameters.RemoveAt(0);
            }

            return parameters;
        }

        /// <summary>
        /// The MyCommand current instance.
        /// </summary>
        public static SuperCommands Instance { get; private set; }
    }
}
