// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

using System;
using System.Windows.Forms;

namespace ReCrafted.Tools.ContentEditor.Content.Importers
{
    /// <summary>
    ///     TextureImport dialog window.
    /// </summary>
    public partial class TextureImportWindow : Form
    {
        public TextureImportWindow()
        {
            InitializeComponent();
        }

        private void Import_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.OK;
        }

        private void Form_Close(object sender, FormClosingEventArgs e)
        {
            if(DialogResult != DialogResult.OK)
                DialogResult = DialogResult.Abort;
        }

        /// <summary>
        ///     When true, importer should generate mipmap chain for this texture.
        /// </summary>
        public bool GenerateMipmaps
        {
            get => Mipmaps.Checked;
            set => Mipmaps.Checked = value;
        }

        /// <summary>
        ///     When true, importer should compress this texture using given compression format.
        /// </summary>
        public bool CompressTexture
        {
            get => Compress.Checked;
            set => Compress.Checked = value;
        }

        /// <summary>
        ///     When true, importer should create meta file near the source file.
        /// </summary>
        public bool CreateMetaFile
        {
            get => Metafile.Checked;
            set => Metafile.Checked = value;
        }
    }
}
