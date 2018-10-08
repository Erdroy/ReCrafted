// ReCrafted Editor (c) 2016-2018 Always Too Late

using System.IO;
using ReCrafted.Editor.Content.Assets;
using ReCrafted.Editor.Utilities;

namespace ReCrafted.Editor.Windows.Content.ContentTree
{
    public class ContentTreeFile
    {
        internal ContentTreeFile(ContentTreeNode parent, string fullName)
        {
            Parent = parent;
            Name = Path.GetFileNameWithoutExtension(fullName);
            FullName = fullName;
        }

        internal ContentTreeFile(ContentTreeNode parent, string name, string fullName)
        {
            Parent = parent;
            Name = name;
            FullName = fullName;
        }

        public void Delete()
        {
            FileSystem.MoveToRecycleBin(FullName);
            Parent.Refresh();
        }

        public void RefreshPreview()
        {

        }

        public void LoadPreview()
        {

        }

        public void GenerateOrLoadPreview()
        {

        }

        public string Name { get; }
        public string FullName { get; }
        public TextureAsset Preview { get; }

        public ContentTreeNode Parent { get; }
    }
}
