namespace ReCrafted.Tools.ContentEditor.Content.Importers
{
    partial class TextureImportWindow
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
            this.Import = new System.Windows.Forms.Button();
            this.Metafile = new System.Windows.Forms.CheckBox();
            this.Mipmaps = new System.Windows.Forms.CheckBox();
            this.Compress = new System.Windows.Forms.CheckBox();
            this.label1 = new System.Windows.Forms.Label();
            this.CompressionFormat = new System.Windows.Forms.ComboBox();
            this.SuspendLayout();
            // 
            // Import
            // 
            this.Import.Location = new System.Drawing.Point(297, 146);
            this.Import.Name = "Import";
            this.Import.Size = new System.Drawing.Size(75, 23);
            this.Import.TabIndex = 0;
            this.Import.Text = "Import";
            this.Import.UseVisualStyleBackColor = true;
            this.Import.Click += new System.EventHandler(this.Import_Click);
            // 
            // Metafile
            // 
            this.Metafile.AutoSize = true;
            this.Metafile.Location = new System.Drawing.Point(192, 150);
            this.Metafile.Name = "Metafile";
            this.Metafile.Size = new System.Drawing.Size(99, 17);
            this.Metafile.TabIndex = 1;
            this.Metafile.Text = "Create meta file";
            this.Metafile.UseVisualStyleBackColor = true;
            // 
            // Mipmaps
            // 
            this.Mipmaps.AutoSize = true;
            this.Mipmaps.Checked = true;
            this.Mipmaps.CheckState = System.Windows.Forms.CheckState.Checked;
            this.Mipmaps.Location = new System.Drawing.Point(15, 75);
            this.Mipmaps.Name = "Mipmaps";
            this.Mipmaps.Size = new System.Drawing.Size(138, 17);
            this.Mipmaps.TabIndex = 2;
            this.Mipmaps.Text = "Generate mipmap chain";
            this.Mipmaps.UseVisualStyleBackColor = true;
            // 
            // Compress
            // 
            this.Compress.AutoSize = true;
            this.Compress.Location = new System.Drawing.Point(15, 52);
            this.Compress.Name = "Compress";
            this.Compress.Size = new System.Drawing.Size(107, 17);
            this.Compress.TabIndex = 3;
            this.Compress.Text = "Compress texture";
            this.Compress.UseVisualStyleBackColor = true;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(99, 13);
            this.label1.TabIndex = 5;
            this.label1.Text = "Compression format";
            // 
            // CompressionFormat
            // 
            this.CompressionFormat.Enabled = false;
            this.CompressionFormat.FormattingEnabled = true;
            this.CompressionFormat.Location = new System.Drawing.Point(15, 25);
            this.CompressionFormat.Name = "CompressionFormat";
            this.CompressionFormat.Size = new System.Drawing.Size(200, 21);
            this.CompressionFormat.TabIndex = 4;
            this.CompressionFormat.Text = "BC7";
            // 
            // TextureImportWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(384, 181);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.CompressionFormat);
            this.Controls.Add(this.Compress);
            this.Controls.Add(this.Mipmaps);
            this.Controls.Add(this.Metafile);
            this.Controls.Add(this.Import);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "TextureImportWindow";
            this.Text = "Texture importer";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form_Close);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button Import;
        private System.Windows.Forms.CheckBox Metafile;
        private System.Windows.Forms.CheckBox Mipmaps;
        private System.Windows.Forms.CheckBox Compress;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox CompressionFormat;
    }
}