// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using System;
using ReCrafted.Tools.APIGenerator.Descriptions;
using ReCrafted.Tools.Common.CodeTokenizer;

namespace ReCrafted.Tools.APIGenerator
{
    public partial class Generator
    {
        private void HandleApiTag(Token token)
        {
            switch (token.Value)
            {
                case "API_CLASS_BODY":
                    _tokenizer.ExpectAllTokens(new[] {TokenType.LeftParent, TokenType.RightParent});
                    break;
                case "API_CLASS":
                    ParseClassTag();
                    break;
                case "API_STRUCT":
                    // TODO: Struct parsing
                    break;
                case "API_FUNCTION":
                    ParseFunctionTag();
                    break;
                case "API_PROPERTY":
                    // TODO: Property parsing
                    break;
                case "API_USING":
                    ParseUsingTag();
                    break;
            }
        }

        private void ParseUsingTag()
        {
            // API_USING("System.Collections.Generic")
            _tokenizer.ExpectToken(TokenType.LeftParent);
            var usingNamespaceToken = _tokenizer.ExpectToken(TokenType.String);
            _tokenizer.ExpectToken(TokenType.RightParent);

            // Cleanup using
            var usingNamespace = usingNamespaceToken.Value.Replace("\"", "");
            
            // TODO: Validate using

            Usings.Add(usingNamespace);
        }

        private void ParseFunctionTag()
        {
            var desc = new FunctionDescription();

            // Comments (read the token before current one)
            var comment = _tokenizer.PreviousToken();
            if (comment.Type == TokenType.CommentMultiLine || comment.Type == TokenType.CommentSingleLine)
                desc.Comment = comment.Value;
            _tokenizer.NextToken();

            // Parse tag arguments
            var tagArguments = ParseTagParameters();

            var token = _tokenizer.NextToken();
            var isStatic = token.Value == "static";

            if (isStatic)
                token = _tokenizer.NextToken();
            
            // Set virtual modifier
            var isVirtual = token.Value == "virtual";

            // If not virtual undo the token
            if (!isVirtual)
                _tokenizer.PreviousToken();

            desc.ReturnType = ParseNativeType();

            var functionName = _tokenizer.ExpectToken(TokenType.Identifier);

            // Set name
            desc.Name = functionName.Value;

            // Parse function parameters
            desc.Parameters.AddRange(ParseFunctionParameters());

            token = _tokenizer.NextToken();

            var isConst = token.Value == "const";

            if (isConst)
                token = _tokenizer.NextToken();

            var isOverride = token.Value == "override";

            if(isOverride)
                Console.WriteLine("WARNING: Function overriding is not supported.");

            for (var i = 0; i < tagArguments.Count; i++)
            {
                token = tagArguments[i];

                switch (token.Value)
                {
                    case "public":
                        desc.Access = AccessModifier.Public;
                        break;
                    case "protected":
                        desc.Access = AccessModifier.Protected;
                        break;
                    case "internal":
                        desc.Access = AccessModifier.Internal;
                        break;
                    case "private":
                        desc.Access = AccessModifier.Private;
                        break;
                    case "static":
                    //case "virtual":
                    //case "override":
                    case "abstract":
                        desc.Modifiers.Add(token.Value);
                        break;
                    default:
                        Console.WriteLine(
                            $"WARNING: Unsupported modifier '{token.Value}' is passed to the API_FUNCTION tag of function {functionName.Value}!");
                        break;
                }
            }

            if (isStatic && !desc.Modifiers.Contains("static"))
                desc.Modifiers.Add("static");

            //if (isOverride && !desc.Modifiers.Contains("override"))
            //    desc.Modifiers.Add("override");

            _functions.Add(desc);
        }

        private void ParseClassTag()
        {
            var desc = new ClassDescription();

            // Comments (read the token before current one)
            var comment = _tokenizer.PreviousToken();
            if (comment.Type == TokenType.CommentMultiLine || comment.Type == TokenType.CommentSingleLine)
                desc.Comment = comment.Value;
            _tokenizer.NextToken();

            // Parse tag arguments
            var tagArguments = ParseTagParameters();

            // Skip until `class` keyword.
            _tokenizer.SkipUntil(TokenType.Identifier);

            // Parse class name
            var className = _tokenizer.ExpectToken(TokenType.Identifier);

            // Set class name
            desc.Name = className.Value;

            // Parse namespace
            desc.Namespace = ParseNamespace(InputFile);

            Console.WriteLine($"C++ class name: '{className.Value}'");

            // Parse class inheritance, support for single Type/Generic Type.
            var token = _tokenizer.NextToken();

            var isFinal = token.Value == "final";

            if (isFinal)
                token = _tokenizer.NextToken();

            if (token.Type == TokenType.Colon)
            {
                // Current class does have inheritance defined
                var accessToken = _tokenizer.ExpectToken(TokenType.Identifier);
                var access = accessToken.Value;

                Token nameToken;
                if (access != "public")
                {
                    Console.WriteLine($"WARNING: C++ class '{className.Value}' inherits a type with non-public access");

                    // Parsed access token is name token, when the 'public' modifier is missing.
                    // (Look at the first comment in this function)
                    nameToken = accessToken;
                }
                else
                {
                    nameToken = _tokenizer.ExpectToken(TokenType.Identifier);
                }

                token = _tokenizer.NextToken();

                if (token.Type == TokenType.LeftAngleBracket)
                {
                    // This is a generic type
                    var genericType = _tokenizer.ExpectToken(TokenType.Identifier);
                    _tokenizer.ExpectToken(TokenType.RightAngleBracket);

                    var inheritType = $"{nameToken.Value}<{genericType.Value}>";

                    Console.WriteLine($"C++ class inherits '{inheritType}'");

                    desc.Inherits = inheritType;
                }
                else
                {
                    var inheritType = nameToken.Value;

                    Console.WriteLine($"C++ class inherits '{inheritType}'");

                    desc.Inherits = inheritType;
                }
            }
            else if(desc.Name != "Object")
            {
                // Must inherit!
                throw new Exception($"Class {className.Value} must inherit Object or any other type that inherits Object!");
            }
            else
            {
                token = _tokenizer.PreviousToken();
            }

            for (var i = 0; i < tagArguments.Count; i++)
            {
                token = tagArguments[i];

                switch (token.Value)
                {
                    case "public":
                        desc.Access = AccessModifier.Public;
                        break;
                    case "protected":
                        desc.Access = AccessModifier.Protected;
                        break;
                    case "internal":
                        desc.Access = AccessModifier.Internal;
                        break;
                    case "private":
                        desc.Access = AccessModifier.Private;
                        break;
                    case "static":
                    case "sealed":
                    case "abstract":
                    case "partial":
                        desc.Modifiers.Add(token.Value);
                        break;
                    case "customNamespace":
                        if (tagArguments.Count == i + 1 || tagArguments[i + 1].Type != TokenType.String)
                            throw new Exception("Class custom namespace doesn't follow with string!");

                        // Override namespace with new one
                        // TODO: Check the namespace for invalid characters etc.
                        desc.Namespace = tagArguments[i + 1].Value.Replace("\"", "");
                        i++;
                        break;

                    default:
                        Console.WriteLine($"WARNING: Unknown class modifier '{token.Value}'.");
                        break;
                }
            }

            if(isFinal && !desc.Modifiers.Contains("sealed"))
                desc.Modifiers.Add("sealed");

            _class = desc;
        }
    }
}
