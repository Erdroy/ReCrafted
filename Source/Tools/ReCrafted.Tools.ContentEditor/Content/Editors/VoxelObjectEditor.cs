// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using System.Windows.Forms;

namespace ReCrafted.Tools.ContentEditor.Content.Editors
{
    public partial class VoxelObjectEditor : Form
    {
        public VoxelObjectEditor()
        {
            InitializeComponent();
        }

        private void Form_Closing(object sender, FormClosingEventArgs e)
        {
            if (DialogResult != DialogResult.OK)
                DialogResult = DialogResult.Abort;
        }
    }
}
