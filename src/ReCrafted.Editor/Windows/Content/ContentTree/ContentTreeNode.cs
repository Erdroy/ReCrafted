// ReCrafted Editor (c) 2016-2018 Always Too Late

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using ReCrafted.Editor.Common;

namespace ReCrafted.Editor.Windows.Content.ContentTree
{
    /// <summary>
    /// Content tree node class.
    /// </summary>
    public partial class ContentTreeNode
    {
        private readonly List<ContentTreeNode> _directories = new List<ContentTreeNode>();
        private readonly List<ContentTreeFile> _files = new List<ContentTreeFile>();
        
        internal ContentTreeNode(ContentTreeNode parent, string path)
        {
            Parent = parent;
            Path = path;
            Name = System.IO.Path.GetFileNameWithoutExtension(path);
            HasCustomName = false;
        }

        internal ContentTreeNode(ContentTreeNode parent, string path, string nodeName)
        {
            Parent = parent;
            Path = path;
            Name = nodeName;
            HasCustomName = true;
        }

        /// <summary>
        /// Renames this directory.
        /// </summary>
        /// <param name="name">The new name of this directory.</param>
        public void Rename(string name)
        {
            Debug.Assert(CanRename, "Cannot rename this node. It's name has been customized or this is virtual path node!");

            throw new NotImplementedException();
        }

        /// <summary>
        /// Deletes this directory and all of it's files.
        /// Can be restored from the system's Trash Bin.
        /// </summary>
        public void Delete()
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Refreshes the directory contents.
        /// </summary>
        public void Refresh()
        {
            var directories = Directory.GetDirectories(Path);
            var files = Directory.GetFiles(Path);
            
            _directories.Clear();
            _files.Clear();

            // Process directories
            foreach (var directory in directories)
            {
                var directoryName = System.IO.Path.GetFileNameWithoutExtension(directory);

                // Skip invalid directory names
                if(string.IsNullOrEmpty(directoryName))
                    continue;
                
                var directoryInfo = new DirectoryInfo(directory);

                // Skip hidden directories
                if (directoryName.StartsWith(".") || (directoryInfo.Attributes & FileAttributes.Hidden) != 0)
                    continue;
                
                // Add the directory
                _directories.Add(new ContentTreeNode(this, directory, directoryName));
            }
            
            // Process files
            foreach (var file in files)
            {
                var isProperAsset = file.EndsWith(".rcasset");
                var fileName = System.IO.Path.GetFileNameWithoutExtension(file);

                // Skip invalid file names
                if (string.IsNullOrEmpty(fileName))
                    continue;

                var fileInfo = new FileInfo(file);

                // Skip hidden files
                if ((fileInfo.Attributes & FileAttributes.Hidden) != 0)
                    continue;

                // Check if this file is proper rcasset.
                if (!isProperAsset)
                {
                    Logger.LogWarning($"Found invalid file '{fileName}' which is not a proper " +
                                      $"ReCrafted asset file. Full path: '{file}'.");
                    continue;
                }

                // Add the file
                var fileNode = new ContentTreeFile(fileName, file);
                _files.Add(fileNode);

                // Generate or load asset preview
                fileNode.GenerateOrLoadPreview();
            }
        }

        /// <summary>
        /// Returns true, when name of this node has been customized and
        /// may not be the same as the directory name contained in path!
        /// </summary>
        public bool HasCustomName { get; }

        /// <summary>
        /// Returns true when this node can be renamed.
        /// </summary>
        public bool CanRename => !HasCustomName;

        /// <summary>
        /// Returns true when this node is the root node.
        /// </summary>
        public bool IsRoot => Parent == null;

        /// <summary>
        /// Name of this node.
        /// Can be different from the original directory name!
        /// </summary>
        public string Name { get; }

        /// <summary>
        /// The path of this node.
        /// Contains the full path name to the containing directory that this node represents.
        /// </summary>
        public string Path { get; }

        /// <summary>
        /// Contains all files owned of this node.
        /// </summary>
        public IReadOnlyList<ContentTreeFile> Files => _files;

        /// <summary>
        /// Contains all sub directories owned of this node.
        /// </summary>
        public IReadOnlyList<ContentTreeNode> SubDirectories => _directories;

        /// <summary>
        /// The parent node. Can be null.
        /// </summary>
        public ContentTreeNode Parent { get; }
    }
}
