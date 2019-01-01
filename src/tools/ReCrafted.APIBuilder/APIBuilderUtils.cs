// ReCrafted (c) 2016-2019 Always Too Late

using System;
using System.Collections.Generic;

namespace ReCrafted.APIBuilder
{
    public static class APIBuilderUtils
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

            var parametersRaw = parameterString.Split(',');

            var list = new List<string>();
            foreach (var parameter in parametersRaw)
            {
                var parameterText = parameter.Trim();

                if (!string.IsNullOrEmpty(parameterText))
                    list.Add(parameterText);
            }

            return list.ToArray();
        }

        public static bool IsString(string parameter)
        {
            return parameter.StartsWith("\"") && parameter.EndsWith("\"");
        }
    }
}
