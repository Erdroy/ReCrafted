// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

using System;
using System.Collections.Generic;
using System.IO;
using ReCrafted.APIBuilder.Tags;

namespace ReCrafted.APIBuilder
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

            { "API_PROPERTY", typeof(APITagProperty) },
            { "API_PROPERTY_END", typeof(APITagPropertyEnd) },

            { "API_METHOD", typeof(APITagMethod) },
            { "API_METHOD_END", typeof(APITagMethodEnd) },
            { "API_CODE", typeof(APITagCode) },
            { "API_PARAM", typeof(APITagParam) },
            { "API_RETURN", typeof(APITagReturn) },
        };
        
        public static List<APITagFile> Files = new List<APITagFile>();
        public static string CurrentFileName;

        public void Build(string sourceFile, string currentDir)
        {
            var filedescs = Parse(sourceFile);

            foreach (var filedesc in filedescs)
            {
                var targetFileName = filedesc.TargetFileName;
                filedesc.SourceFileName = Path.GetFileName(sourceFile);

                var generator = new CodeGenerator
                {
                    Session = new Dictionary<string, object>
                    {
                        { "Desc", filedesc }
                    }
                };
                generator.Initialize();

                var code = generator.TransformText();

                var targetFile = currentDir + "\\src\\ReCrafted.API\\" + targetFileName;

                Directory.CreateDirectory(Path.GetDirectoryName(targetFile));

                //if (new FileInfo(targetFile).LastWriteTime < new FileInfo(sourceFile).LastWriteTime)
                {
                    File.WriteAllText(targetFile, code);
                }
            }
        }

        private APITagFile[] Parse(string fileName)
        {
            CurrentFileName = fileName;

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

                    var inst = (APITag)Activator.CreateInstance(tag);
                    inst.Process(token, parameters);
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"Line {LineNumber} caused an exception {ex}");
                }
            }

            return Files.ToArray();
        }

        public static int LineNumber { get; private set; }

        public static string LineText { get; private set; }
    }
}
