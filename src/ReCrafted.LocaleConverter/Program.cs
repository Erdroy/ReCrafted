// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

using System;
using System.Collections.Generic;
using System.IO;

namespace ReCrafted.LocaleConverter
{
    internal static class Program
    {
        private static void Main(string[] args)
        {
            if (args.Length <= 0) return;

            foreach (var path in args)
            {
                if (!File.Exists(path)) continue;

                // Read data from .csv file
                var file = File.ReadAllText(path);
                var lines = file.Split('\n');

                var data = new Dictionary<string, string>();

                foreach (var line in lines)
                {
                    if (string.IsNullOrEmpty(line)) continue;

                    // var key = line.Substring(0, line.IndexOf(",", StringComparison.Ordinal));
                    // var str = line.Substring(line.IndexOf("\"", StringComparison.Ordinal), line.LastIndexOf("\"", StringComparison.Ordinal) - line.IndexOf("\"", StringComparison.Ordinal));

                    var key = line.Split(';')[0];
                    var str = line.Split(';')[1];

                    data.Add(key, str);
                }

                // Convert data to json
                var json = new MinJson();

                foreach (var record in data)
                {
                    json.Add(record.Key, record.Value);
                }

                // Create json file
                var name = Path.GetFileNameWithoutExtension(path);
                var jsonpath = Path.GetDirectoryName(path) + "/" + name + ".json";

                // Write data to json file
                File.WriteAllText(jsonpath, json.Text);

                Console.WriteLine("Generated file: " + jsonpath);
            }
        }
    }
}