// ReCrafted (c) 2016-2018 Always Too Late

using System;
using System.IO;
using Pdb2Mdb;

namespace ReCrafted.Pdb2MdbConverter
{
    class Program
    {
        static void Main(string[] args)
        {
            var pdbFile = args[0].Trim();

            if (!File.Exists(pdbFile))
            {
                Console.WriteLine($"PDB file '{pdbFile}' not found!");
                return;
            }

            Converter.Convert(pdbFile);
        }
    }
}
