// ReCrafted (c) 2016-2018 Always Too Late

using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using Newtonsoft.Json;

namespace ReCrafted.VersionBuilder
{
    public class GameInfo
    {
        public string BuildName { get; set; }

        public int BuildNumber { get; set; }
    }

    class Program
    {
        static void Main(string[] args)
        {
            // decompiled old tool
            string[] files = Directory.GetFiles(string.Concat(Environment.CurrentDirectory, "..\\project_meta"));
            List<string> list = (
                from file in files
                let fileInfo = new FileInfo(file)
                where fileInfo.Name.StartsWith("buildNum_")
                select file).ToList<string>();
            int num = 0;
            foreach (string str in list)
            {
                num = num + int.Parse(File.ReadAllText(str));
            }
            Console.WriteLine(string.Concat(new object[] { "Found ", list.Count, " build num files. Current build num is: ", num }));
            string str1 = JsonConvert.SerializeObject(new GameInfo()
            {
                BuildName = args[0],
                BuildNumber = num
            }, Formatting.Indented);
            File.WriteAllText(args[1], str1);
        }
    }
}
