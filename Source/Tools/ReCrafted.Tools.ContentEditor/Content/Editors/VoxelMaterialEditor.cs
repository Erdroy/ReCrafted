// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using System.Windows.Forms;

namespace ReCrafted.Tools.ContentEditor.Content.Editors
{
    public partial class VoxelMaterialEditor : Form
    {
        public VoxelMaterialEditor()
        {
            InitializeComponent();
        }

        private void Apply_OnClick(object sender, System.EventArgs e)
        {
            DialogResult = DialogResult.OK;
        }

        private void Revert_OnClick(object sender, System.EventArgs e)
        {
            DialogResult = DialogResult.Abort;
        }

        private void Form_Closing(object sender, FormClosingEventArgs e)
        {
            if (DialogResult != DialogResult.OK)
                DialogResult = DialogResult.Abort;
        }

        private void HardnessScroll_Scroll(object sender, System.EventArgs e)
        {
            HardnessValue.Text = $@"{Hardness} / 255";
        }

        public int Hardness => HardnessScroll.Value;
    }
}
