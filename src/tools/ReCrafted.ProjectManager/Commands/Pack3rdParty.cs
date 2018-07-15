// ReCrafted (c) 2016-2018 Always Too Late

using System;
using System.Collections.Generic;
using System.IO;
using Ionic.Zip;

namespace ReCrafted.ProjectManager.Commands
{
    public class Pack3rdParty : Command
    {
        public override void Execute()
        {
            CreateZip("lib", new List<string>
            {
                "*.c",
                "*.cpp",
                "*.cxx",
                "*.h",
                "*.hpp",
                "*.lib",
                "*.dll",
                "*.def",
                "*.pdb"
            }, "ReCrafted3rdParty.zip");
        }

        /// <summary>
        /// Create zip archive from root directory with search patterns
        /// </summary>
        /// <param name="rootDirectory">Root directory</param>
        /// <param name="searchPatterns">Search patterns</param>
        /// <param name="zipFileName">Zip archive file name</param>
        public static void CreateZip(string rootDirectory, List<string> searchPatterns, string zipFileName)
        {
            var fileNames = GetFileNames(rootDirectory, searchPatterns, true);
            CreateZipFromFileNames(rootDirectory, zipFileName, fileNames);
        }

        /// <summary>
        /// Get file names filtered by search patterns
        /// </summary>
        /// <param name="rootDirectory">Root directory</param>
        /// <param name="searchPatterns">Search patterns</param>
        /// <param name="includeSubdirectories">True if it is included files from sub-directories</param>
        /// <returns>List of file names</returns>
        private static IEnumerable<string> GetFileNames(string rootDirectory, List<string> searchPatterns, bool includeSubdirectories)
        {
            var foundFiles = new List<string>();
            var directoriesToSearch = new Queue<string>();

            directoriesToSearch.Enqueue(rootDirectory);

            // Breadth-First Search
            while (directoriesToSearch.Count > 0)
            {
                var path = directoriesToSearch.Dequeue();

                foreach (var searchPattern in searchPatterns)
                {
                    foundFiles.AddRange(Directory.EnumerateFiles(path, searchPattern));
                }

                if (includeSubdirectories)
                {
                    foreach (var subDirectory in Directory.EnumerateDirectories(path))
                    {
                        directoriesToSearch.Enqueue(subDirectory);
                    }
                }
            }

            return foundFiles;
        }

        // source: https://stackoverflow.com/questions/32206463/how-to-create-zip-archive-contains-files-with-certain-extensions-only
        /// <summary>
        /// Create zip archive from list of file names
        /// </summary>
        /// <param name="rootDirectory">Root directory (for saving required structure of directories)</param>
        /// <param name="zipFileName">File name of zip archive</param>
        /// <param name="fileNames">List of file names</param>
        private static void CreateZipFromFileNames(string rootDirectory, string zipFileName, IEnumerable<string> fileNames)
        {
            var rootZipPath = Directory.GetParent(rootDirectory).FullName;

            var cursorStart = Console.CursorTop;
            using (var zip = new ZipFile(zipFileName))
            {
                zip.SaveProgress += (sender, args) =>
                {
                    Console.SetCursorPosition(0, cursorStart);
                    Console.Write("Progress: " + args.EntriesSaved + " / " + args.EntriesTotal);
                };

                foreach (var filePath in fileNames)
                {
                    var directoryPathInArchive = Path.GetFullPath(Path.GetDirectoryName(filePath)).Substring(rootZipPath.Length);
                    zip.AddFile(filePath, directoryPathInArchive);
                }
                zip.Save();
            }
        }
    }
}
