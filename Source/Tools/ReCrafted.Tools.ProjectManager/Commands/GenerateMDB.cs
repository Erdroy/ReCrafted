
using System;
using System.IO;
using Pdb2Mdb;

namespace ReCrafted.Tools.ProjectManager.Commands
{
    public class GenerateMDB : Command
    {
        public override void Execute()
        {
            var currentDir = Environment.CurrentDirectory;
            if (currentDir.Contains("Tools"))
                currentDir = Path.GetFullPath(currentDir += "\\..");

            // Convert all pdb files
            var bin64 = Path.Combine(currentDir, "Build", "Bin64");
            var pdbFiles = Directory.GetFiles(bin64, "*.dll");

            foreach (var pdb in pdbFiles)
            {
                try
                {
                    Converter.Convert(pdb);
                }
                catch (Exception)
                {
                    // ignored
                }
            }
        }
    }
}
