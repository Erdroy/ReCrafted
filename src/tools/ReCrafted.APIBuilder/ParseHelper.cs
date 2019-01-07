// ReCrafted (c) 2016-2019 Always Too Late

using System.Collections.Generic;

namespace ReCrafted.APIBuilder
{
    public static class ParseHelper
    {
        /// <summary>
        /// Parses standard comma-separated parameters string.
        /// Provides string-aware parsing.
        /// </summary>
        public static string[] ParseParameters(string str)
        {
            var parameters = new List<string>();

            var isString = false;
            var currentParam = string.Empty;
            for (var i = 0; i < str.Length; i++)
            {
                var ch = str[i];

                if (isString)
                {
                    if (ch == '\"')
                    {
                        isString = false;
                        currentParam += ch;
                        parameters.Add(currentParam);
                        currentParam = string.Empty;
                        continue;
                    }

                    currentParam += ch;
                    continue;
                }

                if (ch == '\"')
                {
                    currentParam += ch;
                    isString = true;
                    continue;
                }

                if (ch == ',')
                {
                    parameters.Add(currentParam);
                    currentParam = string.Empty;
                    continue;
                }

                currentParam += ch;
            }

            if (currentParam.Length > 0)
                parameters.Add(currentParam);

            return parameters.ToArray();
        }
    }
}
