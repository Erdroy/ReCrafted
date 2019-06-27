// ReCrafted Editor (c) 2016-2018 Always Too Late

using System;
using ImGuiNET;
using ReCrafted.Editor.Common;
using ReCrafted.Editor.Core;
using Veldrid;
using Veldrid.Sdl2;

namespace ReCrafted.Editor.Windows.Content.ContentTree
{
    public partial class ContentTreeNode
    {
        private bool _lastOpen;
        private bool _forceOpen;

        internal void Open()
        {
            _forceOpen = true;
        }

        /// <summary>
        /// Renders this node tree.
        /// </summary>
        internal void Render(Action<ContentTreeNode> onContextMenuOpen = null)
        {
            // Setup flags
            var flags = SubDirectories.Count == 0 ? ImGuiTreeNodeFlags.Leaf : ImGuiTreeNodeFlags.None | ImGuiTreeNodeFlags.OpenOnArrow;
            if (IsRoot)
                flags |= ImGuiTreeNodeFlags.DefaultOpen;
            
            // Add selected flag when this node is currently open
            if (ContentWindow.Current.CurrentNode == this)
            {
                flags |= ImGuiTreeNodeFlags.Selected;
            }

            // Force open this node if needed
            if (_forceOpen)
            {
                ImGui.SetNextTreeNodeOpen(true);
                _forceOpen = false;
            }

            // Render tree node
            var result = ImGui.TreeNodeEx(Name, flags);
            var noPop = false;

            // Context menu handling
            if (ImGui.IsItemClicked(1))
            {
                onContextMenuOpen?.Invoke(this);
            }

            // Navigation handling
            if (ImGui.IsItemClicked() && ImGui.IsMouseClicked(0))
            {
                // Navigate to this node in the current ContentWindow.
                ContentWindow.Current.Navigate(this);

                // If node is closed, but we get click on our node,
                // we need to force open it.
                if (!result)
                {
                    result = true;
                    noPop = true;
                    _forceOpen = true;
                }
            }

            if (ImGui.IsItemHovered())
            {
                if (Input.IsKeyDown(Key.F2))
                {
                    Logger.Log("Rename!");
                }
            }

            if (result)
            {
                // Refresh this node on every single open after closing or it's initial state.
                if (!_lastOpen)
                {
                    Refresh();
                }
                _lastOpen = true;

                // Render all sub directories
                foreach (var subDir in SubDirectories)
                {
                    subDir.Render(onContextMenuOpen);
                }

                // Pop the node only when TreeNode has been opened by it's API
                // not our click behavior.
                if (!noPop)
                {
                    ImGui.TreePop();
                }

                IsOpen = true;
            }
            else
            {
                // Reset open state, this node has been closed or it is closed.
                _lastOpen = false;

                IsOpen = false;
            }
        }

        public bool IsOpen { get; private set; }
    }
}
