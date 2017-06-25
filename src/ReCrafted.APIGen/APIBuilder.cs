// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

using System;
using System.Collections.Generic;
using System.IO;
using ReCrafted.APIGen.Tags;

namespace ReCrafted.APIGen
{
    internal class APIBuilder
    {
        private readonly Dictionary<string, Type> _apiTags = new Dictionary<string, Type>
        {
            { "API_FILE", typeof(APITagFile) },
            { "API_FILE_END", typeof(APITagFileEnd) },
            { "API_USING", typeof(APITagUsing) },

            { "API_COMMENT", typeof(APITagComment) },

            { "API_CLASS", typeof(APITagClass) },
            { "API_CLASS_END", typeof(APITagClassEnd) },

            { "API_METHOD", typeof(APITagMethod) },
            { "API_METHOD_END", typeof(APITagMethodEnd) },
            { "API_CODE", typeof(APITagMethodCode) },
            { "API_PARAM", typeof(APITagParam) },
            { "API_RETURN", typeof(APITagReturn) },
        };
        
        public string GenerateCode(string fileName, out string targetName)
        {
            var filedesc = Parse(fileName);

            targetName = filedesc.TargetFileName;

            var generator = new CodeGenerator
            {
                Session = new Dictionary<string, object>
                {
                    { "Desc", filedesc }
                }
            };
            generator.Initialize();
            
            return generator.TransformText();
        }

        private APITagFile Parse(string fileName)
        {
            var fileTag = new APITagFile
            {
                SourceFileName = Path.GetFileName(fileName)
            };

            var sourceCode = File.ReadAllText(fileName);
            var lines = sourceCode.Split('\n');

            LineNumber = 0;
            foreach (var lineSource in lines)
            {
                LineNumber++;

                var line = lineSource.Trim();
                LineText = line;
                
                if (!line.StartsWith("API_") || line.StartsWith("API_BIND"))
                    continue;

                try
                {
                    var paramsStart = line.IndexOf("(", StringComparison.Ordinal);
                    var token = line.Substring(0, paramsStart);

                    if (!_apiTags.ContainsKey(token))
                    {
                        Console.WriteLine($"Unknown token '{token}' at line {LineNumber}");
                        continue;
                    }

                    var tag = _apiTags[token];
                    var parameters = APIBuilderUtils.GetParameters(line);

                    if (tag == typeof(APITagFile))
                    {
                        fileTag.Process(token, parameters);
                    }
                    else
                    {
                        var inst = (APITag)Activator.CreateInstance(tag);
                        inst.Process(token, parameters);
                    }
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"Line {LineNumber} caused an exception {ex}");
                }
            }

            return fileTag;
        }

        public static int LineNumber { get; private set; }

        public static string LineText { get; private set; }
    }
}
