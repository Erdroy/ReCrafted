// ReCrafted Editor (c) 2016-2018 Always Too Late

using System.IO;
using ReCrafted.Editor.Content.Assets;
using ReCrafted.Editor.Content.Previews;
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

            RefreshPreview();
        }

        internal ContentTreeFile(ContentTreeNode parent, string name, string fullName)
        {
            Parent = parent;
            Name = name;
            FullName = fullName;

            RefreshPreview();
        }

        public void Delete()
        {
            FileSystem.MoveToRecycleBin(FullName);
            Parent.Refresh();
        }

        public void RefreshPreview()
        {
            Preview = PreviewsCache.Instance.GetPreview(FullName);
        }

        public void LoadPreview()
        {

        }

        public void GenerateOrLoadPreview()
        {

        }

        public string Name { get; }
        public string FullName { get; }
        public TextureAsset Preview { get; private set; }

        public ContentTreeNode Parent { get; }
    }
}
