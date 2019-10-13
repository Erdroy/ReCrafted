namespace ReCrafted.Tools.ContentEditor.Content.Common
{
    partial class AssetFinder
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.ContentView = new System.Windows.Forms.ListView();
            this.AssetFilter = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // ContentView
            // 
            this.ContentView.AllowDrop = true;
            this.ContentView.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.ContentView.AutoArrange = false;
            this.ContentView.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.ContentView.HideSelection = false;
            this.ContentView.LabelEdit = true;
            this.ContentView.Location = new System.Drawing.Point(12, 38);
            this.ContentView.Name = "ContentView";
            this.ContentView.Size = new System.Drawing.Size(620, 391);
            this.ContentView.TabIndex = 1;
            this.ContentView.TileSize = new System.Drawing.Size(256, 60);
            this.ContentView.UseCompatibleStateImageBehavior = false;
            this.ContentView.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.ContentView_MouseDoubleClick);
            // 
            // AssetFilter
            // 
            this.AssetFilter.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.AssetFilter.Location = new System.Drawing.Point(12, 12);
            this.AssetFilter.Name = "AssetFilter";
            this.AssetFilter.Size = new System.Drawing.Size(620, 20);
            this.AssetFilter.TabIndex = 2;
            this.AssetFilter.Text = "t:*; n:*";
            this.AssetFilter.TextChanged += new System.EventHandler(this.TextBox1_TextChanged);
            // 
            // AssetFinder
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(644, 441);
            this.Controls.Add(this.AssetFilter);
            this.Controls.Add(this.ContentView);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.SizableToolWindow;
            this.Name = "AssetFinder";
            this.Text = "Asset Finder";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.AssetFinder_FormClosing);
            this.Load += new System.EventHandler(this.AssetFinder_Load);
            this.ResizeEnd += new System.EventHandler(this.AssetFinder_Resized);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ListView ContentView;
        private System.Windows.Forms.TextBox AssetFilter;
    }
}