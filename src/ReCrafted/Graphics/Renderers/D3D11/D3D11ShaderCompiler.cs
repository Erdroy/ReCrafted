// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

using System;
using System.IO;

namespace ReCrafted.Graphics.Renderers.D3D11
{
    public static class D3D11ShaderCompiler
    {
        public static string ParseShaderSource(string sourceCode, string file)
        {
            var tempSource = sourceCode;
            var sourceFileInfo = new FileInfo(file);

            while (true)
            {
                var includeStart = tempSource.IndexOf("#include", StringComparison.Ordinal);

                if (includeStart != -1)
                {
                    // add includes
                    var start = includeStart + 8;
                    var end = 0;

                    // seek for end
                    var i = start;
                    var open = false;
                    while (true)
                    {
                        if (tempSource[i] == '"')
                        {
                            if (open)
                            {
                                end = i;
                                break;
                            }

                            start = i + 1;
                            open = true;
                        }
                        i++;
                    }

                    // ok
                    var includeFile = sourceFileInfo.DirectoryName + "\\" + tempSource.Substring(start, end - start);

                    tempSource = tempSource.Remove(includeStart, end + 1 - includeStart);
                    tempSource = tempSource.Insert(includeStart, File.ReadAllText(includeFile));
                }
                else
                    break;
            }

            return tempSource;
        }
    }
}