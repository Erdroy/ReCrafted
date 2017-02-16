// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using Newtonsoft.Json;
using ReCrafted.Core;

namespace ReCrafted.VersionBuilder
{
    internal class Program
    {
        private static void Main(string[] args)
        {
            var files = Directory.GetFiles(Environment.CurrentDirectory + "..\\");

            var buildFiles = (
                from file 
                in files
                let fileInfo = new FileInfo(file)
                where fileInfo.Name.StartsWith("buildNum_")
                select file
            ).ToList();

            var buildNum = 0;

            foreach (var file in buildFiles)
            {
                buildNum += int.Parse(File.ReadAllText(file));
            }

            Console.WriteLine("Found " + buildFiles.Count + " build num files. Current build num is: " + buildNum);
            
            var buildinfo = new GameInfo
            {
                BuildName = args[0],
                BuildNumber = buildNum
             };
            var gameinfo = JsonConvert.SerializeObject(buildinfo, Formatting.Indented);
            File.WriteAllText(args[1], gameinfo);
             
        }
    }
}