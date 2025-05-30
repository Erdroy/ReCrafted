﻿// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

using System;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Windows.Forms;
using ReCrafted.Tools.ContentEditor.Content;

namespace ReCrafted.Tools.ContentEditor
{
    /// <summary>
    ///     Content browser class.
    /// </summary>
    public class ContentBrowser
    {
        private class ListViewItemTag
        {
            public string Name;
            public string Path;
            public bool IsFile;
            public bool IsDirectory => !IsFile;
        }

        private readonly string _rootPath;
        private readonly TreeView _contentTree;
        private readonly ListView _contentView;
        private readonly FileSystemWatcher _contentWatcher;

        private string _path;
        private Func<string, bool> _currentEditHandler;

        /// <summary>
        ///     Constructor of the <see cref="ContentBrowser"/> class.
        /// </summary>
        /// <param name="rootPath">The content root path.</param>
        /// <param name="contentTree">The content tree TreeView instance.</param>
        /// <param name="contentView">The content view TreeView instance.</param>
        public ContentBrowser(string rootPath, TreeView contentTree, ListView contentView)
        {
            _rootPath = rootPath;
            _contentTree = contentTree;
            _contentView = contentView;

            _contentWatcher = new FileSystemWatcher(rootPath); // TODO: Use the content FS watcher to update the ContentView and ContentTree accordingly

            contentTree.BeforeExpand += ContentTree_BeforeExpand;
            contentTree.AfterCollapse += ContentTree_AfterCollapse;
            contentTree.AfterSelect += ContentTree_AfterSelect;

            contentView.MouseDoubleClick += ContentView_MouseDoubleClick;

            // Expand the Content node
            var rootNode = contentTree.Nodes[0];
            PopulateTreeViewNode(rootNode);
            rootNode.Expand(); // MAYBE: Load previous expanded nodes
        }

        private void ContentView_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            var senderList = (ListView)sender;
            var clickedItem = senderList.HitTest(e.Location).Item;

            if (clickedItem != null)
            {
                var tag = (ListViewItemTag)clickedItem.Tag;

                if (tag.IsDirectory)
                {
                    // Navigate
                    Navigate(tag.Path);

                    // Open path in ContentTree
                    var localPath = tag.Path.Replace(_rootPath, "");
                    var pathNodeSet = localPath.Split(new[] { "\\" }, StringSplitOptions.RemoveEmptyEntries);

                    // Select root node at first
                    _contentTree.SelectedNode = _contentTree.TopNode;

                    foreach (var nodeName in pathNodeSet)
                    {
                        foreach (TreeNode childNode in _contentTree.SelectedNode.Nodes)
                        {
                            if (childNode.Text == nodeName)
                            {
                                _contentTree.SelectedNode = childNode;
                                childNode.Expand();
                            }
                        }
                    }
                }
                else if (tag.IsFile)
                {
                    // TODO: Open/Edit file
                }
            }
        }

        private void ContentTree_AfterCollapse(object sender, TreeViewEventArgs e)
        {
            // Cleanup nodes
            foreach (TreeNode childrenNode in e.Node.Nodes)
                childrenNode.Nodes.Clear();
        }

        private void ContentTree_BeforeExpand(object sender, TreeViewCancelEventArgs e)
        {
            foreach (TreeNode childrenNode in e.Node.Nodes)
                PopulateTreeViewNode(childrenNode);
        }

        private void ContentTree_AfterSelect(object sender, TreeViewEventArgs e)
        {
            // Navigate the content view
            Navigate(Path.Combine(Settings.Current.GameDirectory, e.Node.FullPath));
        }

        private void PopulateTreeViewNode(TreeNode node)
        {
            var fullPath = Path.Combine(Settings.Current.GameDirectory, node.FullPath);
            var dirInfo = new DirectoryInfo(fullPath);

            foreach (var directory in dirInfo.GetDirectories())
            {
                var nodeName = directory.Name;
                node.Nodes.Add(new TreeNode(nodeName));
            }
        }

        private void OnCreatedItem(object sender, LabelEditEventArgs e)
        {
            // Force proper file name
            var fileName = Path.GetFileNameWithoutExtension(e.Label);
            if (string.IsNullOrEmpty(fileName))
                fileName = "NewAsset";
            fileName += ".rcasset";

            // Update it's name
            _contentView.Items[_contentView.Items.Count - 1].Text = fileName;

            // Call the edit handler and cleanup
            var result = _currentEditHandler(fileName);

            if (!result)
            {
                _contentView.Items.RemoveAt(_contentView.Items.Count - 1);
                e.CancelEdit = true;
            }

            // Cleanup
            _currentEditHandler = null;
            _contentView.AfterLabelEdit -= OnCreatedItem;
        }

        /// <summary>
        ///     Creates new item and begins to edit it's label also invoking <see cref="onCreate"/> delegate.
        /// </summary>
        /// <param name="onCreate">The OnCreate delegate, this is used to process the new item and also eventually cancel it.</param>
        public void CreateItem(Func<string, bool> onCreate)
        {
            _contentView.AfterLabelEdit += OnCreatedItem;
            _currentEditHandler = onCreate;

            var item = _contentView.Items.Add("New Asset");
            item.ImageKey = @"Icon_Unknown";
            item.Selected = true;
            item.BeginEdit();
        }

        /// <summary>
        ///     Refreshes the currently selected directory inside ContentView.
        /// </summary>
        public void Refresh()
        {
            var directory = new DirectoryInfo(CurrentPath);

            var folders = directory.GetDirectories();
            var files = directory.GetFiles();

            // Clear the content view at first.
            _contentView.Items.Clear();

            // Add folders first
            foreach (var folder in folders)
            {
                _contentView.Items.Add(new ListViewItem
                {
                    Text = folder.Name,
                    Tag = new ListViewItemTag
                    {
                        Name = folder.Name,
                        Path = folder.FullName,
                        IsFile = false
                    },
                    ImageIndex = 2
                });
            }

            // And add files just after folders.
            foreach (var file in files)
            {
                var imageKey = PreviewIconManager.Instance.GenerateOrLoad(file.FullName, () =>
                {
                    _contentView.Refresh();
                });

                _contentView.Items.Add(new ListViewItem
                {
                    Text = file.Name,
                    Tag = new ListViewItemTag
                    {
                        Name = file.Name,
                        Path = file.FullName,
                        IsFile = true
                    },
                    ImageKey = imageKey
                });
            }
        }

        /// <summary>
        ///     Navigates to the given path. Uses absolute path, but within <see cref="Settings.GameDirectory"/> path.
        /// </summary>
        /// <param name="path">The absolute path.</param>
        /// <param name="autoRefresh">If true, this call will automatically refresh the content view.</param>
        public void Navigate(string path, bool autoRefresh = true)
        {
            if(path.Length < _rootPath.Length)
                throw new Exception("Invalid path. Cannot exit the root Content directory!");

            _path = path;

            if(autoRefresh)
                Refresh();
        }

        /// <summary>
        ///     Gets or sets the current path.
        /// </summary>
        public string CurrentPath
        {
            get => _path;
            set => Navigate(value);
        }

        /// <summary>
        ///     Gets the absolute base path of the game's content directory.
        /// </summary>
        public string BasePath => Path.GetFullPath(Path.Combine(Settings.Current.GameDirectory, "Content"));
    }
}
