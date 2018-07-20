// ReCrafted (c) 2016-2018 Always Too Late

using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using ReCrafted.ShaderCompiler.Description;
using ReCrafted.CodeTokenizer;

namespace ReCrafted.ShaderCompiler.Compiler
{
    public partial class ShaderCompiler
    {
        private void ParsePass()
        {
            // expect name
            var passName = _parser.ExpectToken(TokenType.Identifier);
            
            if (Options.Current.Verbose)
                Console.WriteLine("Processing pass '" + passName.Value + "'.");

            // expect { after pass name
            _parser.ExpectToken(TokenType.LeftCurlyBrace);

            // construct shader pass
            var pass = new ShaderPass
            {
                Name = passName.Value
            };

            var done = false;
            do
            {
                var token = _parser.NextToken();
                switch (token.Type)
                {
                    case TokenType.Identifier:
                        var functionName = token.Value;
                        var arguments = ParseFunctionArguments();

                        // expect semicolon at the end
                        _parser.ExpectToken(TokenType.SemiColon);

                        switch (functionName)
                        {
                            case "SetProfile":
                            {
                                if(pass.Profile != 0)
                                    throw new Exception("The profile of pass '" + passName + "' is already set!");

                                pass.Profile = ParseSetProfile(functionName, arguments);
                                break;
                            }
                            case "SetDefaultCBTargets":
                            {
                                var targets = ParseSetDefaultCBTargets(functionName, arguments);
                                
                                // Add targets to the default buffer if exists
                                if (Buffers.Count > 0 && (Buffers.First().Name == "DefaultConstants"))
                                {
                                    // Add all targets
                                    Buffers[0].Targets.AddRange(targets);
                                }
                                else
                                {
                                    Console.WriteLine("Warning: Function SetDefaultCBTargets is defined in pass " + passName.Value + " but there is no DefaultConstant buffer included! Ignoring the results.");
                                }
                                break;
                            }
                            case "SetVertexShader":
                            {
                                pass.VSFunction = ParseSetShader(functionName, arguments);
                                break;
                            }
                            case "SetPixelShader":
                            {
                                pass.PSFunction = ParseSetShader(functionName, arguments);
                                break;
                            }
                            case "SetComputeShader":
                            {
                                pass.CSFunction = ParseSetShader(functionName, arguments);
                                break;
                            }

                            default:
                                throw new Exception("Invalid pass function '" + functionName + "' at line " + _parser.CurrentLine + ".");
                        }
                        break;

                    case TokenType.RightCurlyBrace:
                        done = true;
                        break;

                    default:
                        throw new Exception("Expected Identifier or right curly brace at line " + _parser.CurrentLine + ", but got " + token.Type + ".");
                }

            } while (!done);
            
            // add constructed pass
            Passes.Add(pass);
        }

        private int ParseSetProfile(string functionName, IReadOnlyList<Token> arguments)
        {
            if (arguments.Count != 1)
                throw new Exception("Invalid amount of arguments passing to the " + functionName + " at line " + _parser.CurrentLine + ".");

            // get first argument
            var argument = arguments.First();
            var argumentValue = argument.Value;

            // validate profile type
            if (argument.Type != TokenType.Number)
                throw new Exception("Invalid profile passed (" + argumentValue + ") to the SetProfile function at line " + _parser.CurrentLine + "! Available: 2.0 up to 6.0.");

            // parse and check profile
            if (!int.TryParse(argumentValue, NumberStyles.Any, CultureInfo.InvariantCulture, out var profile) || profile < 2 || profile > 6)
                throw new Exception("Invalid profile passed (" + argumentValue + ") to the SetProfile function at line " + _parser.CurrentLine + "! Available: 2.0 up to 6.0.");
            
            return profile;
        }

        private string[] ParseSetDefaultCBTargets(string functionName, IReadOnlyList<Token> arguments)
        {
            if (arguments.Count == 0)
                throw new Exception("Invalid amount of arguments passing to the " + functionName + " at line " + _parser.CurrentLine + ".");

            var targets = new List<string>();

            // Validate and select arguments
            foreach (var argument in arguments)
            {
                // validate profile type
                if (argument.Type != TokenType.Identifier)
                    throw new Exception("Invalid profile passed (" + argument + ") to the SetDefaultCBTargets function at line " + _parser.CurrentLine + ". Expected identifier.");

                targets.Add(argument.Value);
            }

            return targets.ToArray();
        }

        private string ParseSetShader(string functionName, IReadOnlyList<Token> arguments)
        {
            if (arguments.Count != 1)
                throw new Exception("Invalid amount of arguments passing to the " + functionName + " at line " + _parser.CurrentLine + ".");

            // get first argument
            var argument = arguments.First();
            var argumentValue = argument.Value;

            // validate profile type
            if (argument.Type != TokenType.Identifier)
                throw new Exception("Invalid shader function name passed (" + argumentValue + ") to the SetShader function at line " + _parser.CurrentLine + "!");

            // set shader function
            return argumentValue;
        }
    }
}
