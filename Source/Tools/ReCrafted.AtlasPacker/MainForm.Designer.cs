namespace ReCrafted.AtlasPacker
{
    partial class MainForm
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
            this.listView1 = new System.Windows.Forms.ListView();
            this.label1 = new System.Windows.Forms.Label();
            this.maxSize = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.addImage = new System.Windows.Forms.Button();
            this.export = new System.Windows.Forms.Button();
            this.method = new System.Windows.Forms.ComboBox();
            this.label3 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // listView1
            // 
            this.listView1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.listView1.Location = new System.Drawing.Point(12, 28);
            this.listView1.Name = "listView1";
            this.listView1.Size = new System.Drawing.Size(551, 534);
            this.listView1.TabIndex = 0;
            this.listView1.UseCompatibleStateImageBehavior = false;
            this.listView1.View = System.Windows.Forms.View.List;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 12);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(80, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Images to pack";
            // 
            // maxSize
            // 
            this.maxSize.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.maxSize.Location = new System.Drawing.Point(572, 44);
            this.maxSize.Name = "maxSize";
            this.maxSize.Size = new System.Drawing.Size(78, 20);
            this.maxSize.TabIndex = 2;
            this.maxSize.Text = "1024";
            this.maxSize.TextChanged += new System.EventHandler(this.maxSize_TextChanged);
            // 
            // label2
            // 
            this.label2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(569, 28);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(159, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = "Max size (power of two - square)";
            // 
            // addImage
            // 
            this.addImage.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.addImage.Location = new System.Drawing.Point(572, 487);
            this.addImage.Name = "addImage";
            this.addImage.Size = new System.Drawing.Size(156, 24);
            this.addImage.TabIndex = 4;
            this.addImage.Text = "Add image(s)";
            this.addImage.UseVisualStyleBackColor = true;
            this.addImage.Click += new System.EventHandler(this.addImage_Click);
            // 
            // export
            // 
            this.export.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.export.Location = new System.Drawing.Point(572, 517);
            this.export.Name = "export";
            this.export.Size = new System.Drawing.Size(156, 45);
            this.export.TabIndex = 5;
            this.export.Text = "Export";
            this.export.UseVisualStyleBackColor = true;
            this.export.Click += new System.EventHandler(this.export_Click);
            // 
            // method
            // 
            this.method.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.method.FormattingEnabled = true;
            this.method.Items.AddRange(new object[] {
            "Basic"});
            this.method.Location = new System.Drawing.Point(572, 94);
            this.method.Name = "method";
            this.method.Size = new System.Drawing.Size(92, 21);
            this.method.TabIndex = 6;
            this.method.Text = "Basic";
            // 
            // label3
            // 
            this.label3.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(569, 78);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(43, 13);
            this.label3.TabIndex = 7;
            this.label3.Text = "Method";
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(740, 574);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.method);
            this.Controls.Add(this.export);
            this.Controls.Add(this.addImage);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.maxSize);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.listView1);
            this.Name = "MainForm";
            this.Text = "ReCrafted Atlas Packer";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ListView listView1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox maxSize;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button addImage;
        private System.Windows.Forms.Button export;
        private System.Windows.Forms.ComboBox method;
        private System.Windows.Forms.Label label3;
    }
}

