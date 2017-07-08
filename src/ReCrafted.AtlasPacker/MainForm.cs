using System;
using System.Linq;
using System.Windows.Forms;

namespace ReCrafted.AtlasPacker
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();
        }

        private void addImage_Click(object sender, EventArgs e)
        {
            var openFileDialog = new OpenFileDialog
            {
                Multiselect = true,
                CheckFileExists = true,
                Filter = @"Image files (*.jpg, *.png) | *.jpg; *.png"
            };

            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                foreach (var file in openFileDialog.FileNames)
                {
                    listView1.Items.Add(new ListViewItem(file));
                }
            }
            listView1.Refresh();
        }

        private void export_Click(object sender, EventArgs e)
        {
            var imageFiles = (from object item in listView1.Items select item.ToString()).ToList();

            // process all images (build rectangles)
            // create texture
            // pack, use image name as key
        }

        private void maxSize_TextChanged(object sender, EventArgs e)
        {

        }
    }
}
