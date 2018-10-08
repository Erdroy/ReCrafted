// ReCrafted Editor (c) 2016-2018 Always Too Late

using System.IO;
using ReCrafted.Editor.Content.Assets;

namespace ReCrafted.Editor.Windows.Content.ContentTree
{
    public class ContentTreeFile
    {
        internal ContentTreeFile(string fullName)
        {
            Name = Path.GetFileNameWithoutExtension(fullName);
            FullName = fullName;
        }

        internal ContentTreeFile(string name, string fullName)
        {
            Name = name;
            FullName = fullName;
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
    }
}
