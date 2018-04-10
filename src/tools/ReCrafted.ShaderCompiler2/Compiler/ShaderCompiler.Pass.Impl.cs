// GFXL - Graphics Library (c) 2016-2017 Damian 'Erdroy' Korczowski

using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using ReCrafted.ShaderCompiler.Description;
using ReCrafted.ShaderCompiler.Tokenizer;

namespace ReCrafted.ShaderCompiler.Compiler
{
    public partial class ShaderCompiler
    {
        private void ParsePass()
        {
            // expect name
            var passName = ExpectToken(TokenType.Identifier);
            
            if (Options.Current.Verbose)
                Console.WriteLine("Processing pass '" + passName.Value + "'.");
            
            // expect { after pass name
            ExpectToken(TokenType.LeftCurlyBrace);

            // construct shader pass
            var pass = new ShaderPass
            {
                Name = passName.Value
            };

            var done = false;
            do
            {
                var token = NextToken();
                switch (token.Type)
                {
                    case TokenType.Identifier:
                        var functionName = token.Value;
                        var arguments = ParseFunctionArguments();

                        // expect semicolon at the end
                        ExpectToken(TokenType.SemiColon);

                        switch (functionName)
                        {
                            case "SetProfile":
                            {
                                if(pass.Profile != 0)
                                    throw new Exception("The profile of pass '" + passName + "' is already set!");

                                pass.Profile = ParseSetProfile(functionName, arguments);
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
                                throw new Exception("Invalid pass function '" + functionName + "' at line " + _currentLineNumber + ".");
                        }
                        break;

                    case TokenType.RightCurlyBrace:
                        done = true;
                        break;

                    default:
                        throw new Exception("Expected Identifier or right curly brace at line " + _currentLineNumber + ", but got " + token.Type + ".");
                }

            } while (!done);
            
            // add constructed pass
            Passes.Add(pass);
        }

        private int ParseSetProfile(string functionName, IReadOnlyList<Token> arguments)
        {
            if (arguments.Count != 1)
                throw new Exception("Invalid amount of arguments passing to the " + functionName + " at line " + _currentLineNumber + ".");

            // get first argument
            var argument = arguments.First();
            var argumentValue = argument.Value;

            // validate profile type
            if (argument.Type != TokenType.Number)
                throw new Exception("Invalid profile passed (" + argumentValue + ") to the SetProfile function at line " + _currentLineNumber + "! Available: 2.0 up to 6.0.");

            // parse and check profile
            if (!int.TryParse(argumentValue, NumberStyles.Any, CultureInfo.InvariantCulture, out var profile) || profile < 2 || profile > 6)
                throw new Exception("Invalid profile passed (" + argumentValue + ") to the SetProfile function at line " + _currentLineNumber + "! Available: 2.0 up to 6.0.");
            
            return profile;
        }

        private string ParseSetShader(string functionName, IReadOnlyList<Token> arguments)
        {
            if (arguments.Count != 1)
                throw new Exception("Invalid amount of arguments passing to the " + functionName + " at line " + _currentLineNumber + ".");

            // get first argument
            var argument = arguments.First();
            var argumentValue = argument.Value;

            // validate profile type
            if (argument.Type != TokenType.Identifier)
                throw new Exception("Invalid shader function name passed (" + argumentValue + ") to the SetShader function at line " + _currentLineNumber + "!");

            // set shader function
            return argumentValue;
        }
    }
}
