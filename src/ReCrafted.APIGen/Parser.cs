// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace ReCrafted.APIGen
{
    // ReSharper disable once InconsistentNaming
    public class IAPIObject { }

    public class PropertyDescription : IAPIObject
    {
        // TODO: properties
    }

    public class MethodParameterDescription : IAPIObject
    {
        public enum Type
        {
            Regular,
            Out,
            Ref
        }

        public string Name { get; set; }

        public string TypeName { get; set; }

        public string Comment { get; set; }

        public Type ParamType { get; set; }
    }

    public class MethodReturnDescription : IAPIObject
    {
        public string Name { get; set; }

        public string TypeName { get; set; }

        public string Comment { get; set; }
    }

    public class MethodDescription : IAPIObject
    {
        public enum Type
        {
            Regular,
            Static,
            Abstract,
            Virtual
        }

        public string Name { get; set; }

        public string Comment { get; set; }

        public bool Overrides { get; set; }

        public bool ReturnsVoid { get; set; }

        public Type MethodType { get; set; }

        public MethodParameterDescription[] ParameterDescriptions;

        public MethodReturnDescription ReturnDescription;
    }

    public class ObjectDescription : IAPIObject
    {
        public enum Type
        {
            Regular,
            Static,
            Abstract,
            Sealed
        }

        public enum Keyword
        {
            Class,
            Struct,
            Interface,
            Enum // enum uses 'PropertyDescriptions' for definition
        }

        public string Namespace { get; set; }

        public string Name { get; set; }

        public string Inherits { get; set; }

        public string Comment { get; set; }

        public Type ClassType { get; set; }

        public Keyword KeywordType { get; set; }

        public PropertyDescription[] PropertyDescriptions { get; set; }

        public MethodDescription[] MethodDescriptions { get; set; }
    }

    public class SourceFileDescription : IAPIObject
    {
        public string FileName { get; set; }

        public ObjectDescription ObjectDescription { get; set; }
    }

    public static class Parser
    {
        public static string GetStringValue(string str)
        {
            var start = str.IndexOf("\"", StringComparison.Ordinal);
            var end = str.LastIndexOf("\"", StringComparison.Ordinal);
            return str.Substring(start + 1, end - 1 - start);
        }
        
        public static string[] GetParameters(string line)
        {
            var start = line.IndexOf("(", StringComparison.Ordinal);
            var end = line.LastIndexOf(")", StringComparison.Ordinal);
            var parameterString = line.Substring(start + 1, end - 1 - start);

            // "ReCrafted.Core", "Logger", SEALED, INHERIT("ILogger")

            var parametersRaw = parameterString.Split(',');

            //  "ReCrafted.Core" 
            // "Logger" 
            //  SEALED
            // INHERIT("ILogger")

            var list = new List<string>();
            foreach (var parameter in parametersRaw)
            {
                var parameterText = parameter.Trim();
                
                if(!string.IsNullOrEmpty(parameterText))
                    list.Add(parameterText);
            }

            return list.ToArray();
        }

        public static SourceFileDescription[] ParseFile(string file)
        {
            var contents = File.ReadAllText(file);
            var lines = contents.Split('\n');
            var lineNumber = 0;

            var sourceFiles = new List<SourceFileDescription>();

            SourceFileDescription currentSource = null;
            ObjectDescription currentClass = null;
            MethodDescription currentMethod = null;
            IAPIObject currentObject = null;

            var comment = "";
            foreach (var sourceLine in lines)
            {
                lineNumber++;

                try
                {
                    var line = sourceLine.Trim();

                    if (line.StartsWith("API_BIND") || !line.StartsWith("API_")) // ignore binds or non-api lines
                        continue;

                    var commentIndex = line.IndexOf("//", StringComparison.Ordinal);

                    if (commentIndex > 0)
                        line = line.Remove(commentIndex, line.Length - commentIndex);

                    var openbracket = line.IndexOf("(", StringComparison.Ordinal);

                    if (openbracket <= 0)
                    {
                        Console.WriteLine("Invalid API token: " + line);
                        continue;
                    }

                    var token = line.Substring(0, openbracket);
                    var parameters = GetParameters(line);

                    switch (token)
                    {
                        case "API_BEGIN": // ok
                            {
                                if (parameters.Length != 1)
                                {
                                    Console.WriteLine("Invalid API_BEGIN at line: " + lineNumber + ", in file: " + file + "!");
                                    continue;
                                }

                                var sourceFileName = GetStringValue(parameters[0]);

                                var obj = new SourceFileDescription
                                {
                                    FileName = sourceFileName
                                };

                                currentSource = obj;
                                currentObject = obj;
                            }
                            break;
                        case "API_END": // ok
                            if (currentObject != null)
                            {
                                sourceFiles.Add(currentSource);
                            }
                            break;
                        case "API_PUSH":
                            {
                                // push current object
                                if (currentObject is ObjectDescription)
                                {
                                    currentSource.ObjectDescription = (ObjectDescription)currentObject;
                                }
                                else if (currentObject is MethodDescription)
                                {
                                    var list = new List<MethodDescription>(currentClass.MethodDescriptions)
                                    {
                                        (MethodDescription) currentObject
                                    };
                                    currentClass.MethodDescriptions = list.ToArray();
                                }
                                // TODO: impl all types
                            }
                            break;
                        case "API_COMMENT": // ok
                            comment = GetStringValue(parameters[0]);
                            break;
                        case "API_CLASS": // ok
                            {
                                var obj = new ObjectDescription
                                {
                                    Namespace = GetStringValue(parameters[0]),
                                    Name = GetStringValue(parameters[1]),
                                    Comment = comment,
                                    KeywordType = ObjectDescription.Keyword.Class,
                                    MethodDescriptions = new MethodDescription[0],
                                    PropertyDescriptions = new PropertyDescription[0]
                                };

                                // select class type
                                if (parameters.Contains("SEALED"))
                                    obj.ClassType = ObjectDescription.Type.Sealed;
                                else if (parameters.Contains("ABSTRACT"))
                                    obj.ClassType = ObjectDescription.Type.Abstract;
                                else if(parameters.Contains("STATIC"))
                                    obj.ClassType = ObjectDescription.Type.Static;
                                else
                                    obj.ClassType = ObjectDescription.Type.Regular;

                                // inherits
                                if (parameters.Any(x => x.Contains("INHERIT")))
                                {
                                    var param = parameters.First(x => x.Contains("INHERIT"));
                                    var paramsArray = GetParameters(param);
                                    var baseClass = GetStringValue(paramsArray[0]);

                                    // class intherits some type
                                    obj.Inherits = baseClass;
                                }

                                comment = "";
                                currentObject = obj;
                                currentClass = obj;
                            }
                            break;
                        case "API_METHOD": // ok
                            {
                                var obj = new MethodDescription
                                {
                                    Name = parameters[0],
                                    Comment = comment,
                                    Overrides = parameters.Contains("OVERRIDE"),
                                    ReturnsVoid = true,
                                    ParameterDescriptions = new MethodParameterDescription[0]
                                };

                                // select class type
                                if (parameters.Contains("ABSTRACT"))
                                    obj.MethodType = MethodDescription.Type.Abstract;
                                else if (parameters.Contains("VIRTUAL"))
                                    obj.MethodType = MethodDescription.Type.Virtual;
                                else if (parameters.Contains("STATIC"))
                                    obj.MethodType = MethodDescription.Type.Static;
                                else
                                    obj.MethodType = MethodDescription.Type.Regular;

                                // add
                                var list = new List<MethodDescription>(currentClass.MethodDescriptions)
                                {
                                    obj
                                };
                                currentClass.MethodDescriptions = list.ToArray();

                                comment = "";
                                currentMethod = obj;
                                currentObject = currentMethod;
                            }
                            break;

                        case "API_PARAM": // ok
                            {
                                var obj = new MethodParameterDescription
                                {
                                    Comment = comment,
                                    TypeName = GetStringValue(parameters[0]),
                                    Name = GetStringValue(parameters[1]),
                                    ParamType = MethodParameterDescription.Type.Regular
                                };
                                comment = "";

                                var list = new List<MethodParameterDescription>(currentMethod.ParameterDescriptions)
                                {
                                    obj
                                };
                                currentMethod.ParameterDescriptions = list.ToArray();
                            }
                            break;
                        case "API_PARAM_REF": // ok
                            {
                                var obj = new MethodParameterDescription
                                {
                                    Comment = comment,
                                    TypeName = GetStringValue(parameters[0]),
                                    Name = GetStringValue(parameters[1]),
                                    ParamType = MethodParameterDescription.Type.Ref
                                };
                                comment = "";

                                var list = new List<MethodParameterDescription>(currentMethod.ParameterDescriptions)
                                {
                                    obj
                                };
                                currentMethod.ParameterDescriptions = list.ToArray();
                            }
                            break;
                        case "API_PARAM_OUT": // ok
                            {
                                var obj = new MethodParameterDescription
                                {
                                    Comment = comment,
                                    TypeName = GetStringValue(parameters[0]),
                                    Name = GetStringValue(parameters[1]),
                                    ParamType = MethodParameterDescription.Type.Out
                                };
                                comment = "";

                                var list = new List<MethodParameterDescription>(currentMethod.ParameterDescriptions)
                                {
                                    obj
                                };
                                currentMethod.ParameterDescriptions = list.ToArray();
                            }
                            break;
                        case "API_RETURN":
                            //currentObject = new MethodReturnDescription();
                            // set currentMethod.ReturnsVoid = false
                            // TODO
                            break;
                    }
                }
                catch (Exception ex)
                {
                    Console.WriteLine("Line " + lineNumber + " caused an exception: " + ex);
                }
            }

            return sourceFiles.ToArray();
        }
    }
}