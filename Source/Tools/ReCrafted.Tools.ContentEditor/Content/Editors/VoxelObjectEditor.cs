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

        private void Apply_Click(object sender, System.EventArgs e)
        {
            DialogResult = DialogResult.OK;
            Close();
        }

        private void Revert_Click(object sender, System.EventArgs e)
        {
            DialogResult = DialogResult.Abort;
            Close();
        }

        public string TopHeightMap => TexTop.SelectedAsset.AssetFile;
        public string BottomHeightMap => TexBottom.SelectedAsset.AssetFile;
        public string LeftHeightMap => TexLeft.SelectedAsset.AssetFile;
        public string RightHeightMap => TexRight.SelectedAsset.AssetFile;
        public string FrontHeightMap => TexTop.SelectedAsset.AssetFile;
        public string BackHeightMap => TexTop.SelectedAsset.AssetFile;

        public string VoxelObjectName => ObjectName.Text;
    }
}
