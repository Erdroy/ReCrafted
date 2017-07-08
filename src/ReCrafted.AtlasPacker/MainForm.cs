using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Windows.Forms;
using Newtonsoft.Json;

namespace ReCrafted.AtlasPacker
{
    public struct SerializationRect
    {
        public string Name;
        public int X;
        public int Y;
        public int Width;
        public int Height;
    }

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
            var size = int.Parse(maxSize.Text);

            var imageFiles = (from object item in listView1.Items select ((ListViewItem)item).Text).ToList();
            var images = new List<Image>();
            var rectangles = new List<Rect>();
            var bin = new MaxRectsBinPack(size, size, false);

            // TODO: refactor
            // TODO: more build methods

            // process all images (build rectangles)
            foreach (var file in imageFiles)
            {
                var img = Image.FromFile(file);
                images.Add(img);
                rectangles.Add(bin.Insert(img.Width, img.Height, MaxRectsBinPack.FreeRectChoiceHeuristic.RectBestAreaFit));
            }

            // create output texture
            var outputImg = new Bitmap(size, size);
            using (var gfx = Graphics.FromImage(outputImg))
            {
                for (var i = 0; i < images.Count; i++)
                {
                    var img = images[i];
                    var rect = rectangles[i];

                    gfx.DrawImage(img, new PointF(rect.X, rect.Y));
                }
            }

            // save output
            var outputSaveDialog = new SaveFileDialog
            {
                Filter = @"Image file *.png | *.png"
            };

            if (outputSaveDialog.ShowDialog() == DialogResult.OK)
            {
                outputImg.Save(outputSaveDialog.FileName);

                // build and save json
                var list = new List<SerializationRect>();

                var idx = 0;
                foreach (var rect in rectangles)
                {
                    var imgName = imageFiles[idx];
                    var name = Path.GetFileNameWithoutExtension(imgName);

                    list.Add(new SerializationRect
                    {
                        Name = name,
                        Width = (int)rect.Width,
                        Height = (int)rect.Height,
                        X = (int)rect.X,
                        Y = (int)rect.Y,
                    });

                    idx++;
                }

                var jsonFile = JsonConvert.SerializeObject(list, Formatting.Indented);
                File.WriteAllText(outputSaveDialog.FileName.Replace(".png", ".json"), jsonFile);
            }
        }

        private void maxSize_TextChanged(object sender, EventArgs e)
        {

        }
    }
}
