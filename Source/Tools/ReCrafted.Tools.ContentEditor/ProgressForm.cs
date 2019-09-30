// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using System.Windows.Forms;

namespace ReCrafted.Tools.ContentEditor
{
    public partial class ProgressForm : Form
    {
        public ProgressForm()
        {
            InitializeComponent();
        }

        /// <summary>
        ///     The progress bar value
        /// </summary>
        public float Progress
        {
            get => progressBar1.Value / 100.0f;
            set => progressBar1.Value = (int) (value * 100);
        }
    }
}
